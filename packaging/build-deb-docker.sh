#!/bin/bash
# Build Debian packages using Docker
# This script runs on macOS and uses Docker to build .deb packages

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "====================================="
echo "Docker-based Debian Package Build"
echo "====================================="

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo "ERROR: Docker is not running. Please start Docker Desktop."
    exit 1
fi

# Build the Docker image
echo "Building Debian build environment..."
docker build -t faircom-fdw-deb-builder -f "$SCRIPT_DIR/Dockerfile.debian" "$SCRIPT_DIR"

# Run the build in Docker
echo "Running package build in container..."
docker run --rm \
    -v "$PROJECT_ROOT:/project:ro" \
    -v "$PROJECT_ROOT/build:/build" \
    -w /build \
    faircom-fdw-deb-builder \
    bash -c "
        set -e
        echo 'Copying source files...'
        rm -rf /build/pkg
        mkdir -p /build/pkg
        cp -r /project/core /build/pkg/
        cp -r /project/packaging/debian /build/pkg/
        cp /project/README.md /build/pkg/

        cd /build/pkg

        echo ''
        echo '====================================='
        echo 'Building PostgreSQL 14 package'
        echo '====================================='

        dpkg-buildpackage -us -uc -b || {
            echo 'ERROR: Build failed'
            exit 1
        }

        # Move packages to output
        mkdir -p /build/output
        mv ../*.deb /build/output/ 2>/dev/null || true
        mv ../*.ddeb /build/output/ 2>/dev/null || true
        mv ../*.changes /build/output/ 2>/dev/null || true
        mv ../*.buildinfo /build/output/ 2>/dev/null || true

        echo ''
        echo '====================================='
        echo 'Build complete!'
        echo '====================================='
        ls -lh /build/output/*.deb /build/output/*.ddeb 2>/dev/null || echo 'No packages created'
    "

# Check results
if [ -d "$PROJECT_ROOT/build/output" ]; then
    echo ""
    echo "Packages created:"
    ls -lh "$PROJECT_ROOT/build/output"/*.deb "$PROJECT_ROOT/build/output"/*.ddeb 2>/dev/null || echo "No .deb files found"
else
    echo "ERROR: No output directory created"
    exit 1
fi
