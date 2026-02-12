#!/bin/bash
# Build RPM packages using Docker
# This script runs on macOS and uses Docker to build .rpm packages

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "====================================="
echo "Docker-based RPM Package Build"
echo "====================================="

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo "ERROR: Docker is not running. Please start Docker Desktop."
    exit 1
fi

# Build the Docker image
echo "Building RPM build environment..."
docker build -t faircom-fdw-rpm-builder -f "$SCRIPT_DIR/Dockerfile.rpm" "$SCRIPT_DIR"

# Run the build in Docker
echo "Running package build in container..."
docker run --rm \
    -v "$PROJECT_ROOT:/project:ro" \
    -v "$PROJECT_ROOT/build:/build" \
    -w /build \
    faircom-fdw-rpm-builder \
    bash -c "
        set -e

        VERSION=\"$VERSION\"
        if [ -z \"\$VERSION\" ] && [ -f /project/packaging/debian/changelog ]; then
            VERSION=\"\$(awk -F'[()]' 'NR==1{print \$2}' /project/packaging/debian/changelog | cut -d- -f1)\"
        fi
        if [ -z \"\$VERSION\" ]; then
            echo 'ERROR: VERSION is not set and could not be inferred.'
            exit 1
        fi

        # Setup RPM build environment
        mkdir -p /build/rpm/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

        # Create source tarball
        echo 'Creating source tarball...'
        TARBALL_NAME=\"faircom_fdw-\$VERSION\"
        mkdir -p /build/rpm/SOURCES/\$TARBALL_NAME
        cp -r /project/core /build/rpm/SOURCES/\$TARBALL_NAME/
        if [ -f /project/README.md ]; then
            cp /project/README.md /build/rpm/SOURCES/\$TARBALL_NAME/
        fi
        if [ -f /project/LICENSE ]; then
            cp /project/LICENSE /build/rpm/SOURCES/\$TARBALL_NAME/
        fi

        cd /build/rpm/SOURCES
        tar czf \$TARBALL_NAME.tar.gz \$TARBALL_NAME
        rm -rf \$TARBALL_NAME

        # Copy spec file
        cp /project/packaging/rpm/faircom_fdw.spec /build/rpm/SPECS/
        sed -i "s/@VERSION@/\$VERSION/g" /build/rpm/SPECS/faircom_fdw.spec

        # Build only for PostgreSQL 14 (current dev target)
        for PG_VERSION in 14; do
            echo ''
            echo '====================================='
            echo 'Building for PostgreSQL '\$PG_VERSION
            echo '====================================='

            rpmbuild \
                --define \"_topdir /build/rpm\" \
                --define \"pgmajorversion \$PG_VERSION\" \
                -bb /build/rpm/SPECS/faircom_fdw.spec || {
                echo \"ERROR: Build failed for PostgreSQL \$PG_VERSION\"
                continue
            }
        done

        # Move packages to output
        mkdir -p /build/output
        find /build/rpm/RPMS -name '*.rpm' -exec mv {} /build/output/ \;

        echo ''
        echo '====================================='
        echo 'Build complete!'
        echo '====================================='
        ls -lh /build/output/*.rpm 2>/dev/null || echo 'No packages created'
    "

# Check results
if [ -d "$PROJECT_ROOT/build/output" ]; then
    echo ""
    echo "Packages created:"
    ls -lh "$PROJECT_ROOT/build/output"/*.rpm 2>/dev/null || echo "No .rpm files found"
else
    echo "ERROR: No output directory created"
    exit 1
fi
