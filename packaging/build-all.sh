#!/bin/bash
# Build all package types for FairCom FDW
# Supports Debian (.deb) and RPM (.rpm) packages

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "FairCom FDW - Multi-Platform Package Build"
echo "=========================================="
echo ""

# Parse arguments
BUILD_DEB=true
BUILD_RPM=true
ARCH=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --deb-only)
            BUILD_RPM=false
            shift
            ;;
        --rpm-only)
            BUILD_DEB=false
            shift
            ;;
        --arch)
            ARCH="$2"
            shift 2
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --deb-only      Build only Debian packages"
            echo "  --rpm-only      Build only RPM packages"
            echo "  --arch ARCH     Target architecture (amd64, x86_64, arm64)"
            echo "  --help          Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                    # Build all packages"
            echo "  $0 --deb-only         # Build only .deb packages"
            echo "  $0 --arch arm64       # Build for ARM64 architecture"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Run with --help for usage information"
            exit 1
            ;;
    esac
done

# Detect OS and check build capability
OS_TYPE=""
if [ -f /etc/debian_version ]; then
    OS_TYPE="debian"
    echo "Detected Debian/Ubuntu system"
elif [ -f /etc/redhat-release ]; then
    OS_TYPE="redhat"
    echo "Detected RHEL/CentOS/Fedora system"
else
    echo "WARNING: Cannot detect OS type. Build may fail."
fi

# Build Debian packages
if [ "$BUILD_DEB" = true ]; then
    echo ""
    echo "=========================================="
    echo "Building Debian packages"
    echo "=========================================="

    if [ -n "$ARCH" ]; then
        export ARCH="$ARCH"
    fi

    if [ "$OS_TYPE" != "debian" ]; then
        echo "WARNING: Building .deb packages on non-Debian system may fail"
        echo "Consider using a Debian/Ubuntu build environment"
    fi

    bash "$SCRIPT_DIR/build-deb.sh"
fi

# Build RPM packages
if [ "$BUILD_RPM" = true ]; then
    echo ""
    echo "=========================================="
    echo "Building RPM packages"
    echo "=========================================="

    if [ -n "$ARCH" ]; then
        export ARCH="$ARCH"
    fi

    if [ "$OS_TYPE" != "redhat" ]; then
        echo "WARNING: Building .rpm packages on non-RHEL system may fail"
        echo "Consider using a RHEL/CentOS/Fedora build environment"
    fi

    bash "$SCRIPT_DIR/build-rpm.sh"
fi

echo ""
echo "=========================================="
echo "Build Summary"
echo "=========================================="

if [ "$BUILD_DEB" = true ]; then
    echo ""
    echo "Debian packages (.deb):"
    find "$SCRIPT_DIR/../build/debian/output" -name "*.deb" 2>/dev/null || echo "  No .deb packages found"
fi

if [ "$BUILD_RPM" = true ]; then
    echo ""
    echo "RPM packages (.rpm):"
    find "$SCRIPT_DIR/../build/rpm/output" -name "*.rpm" 2>/dev/null || echo "  No .rpm packages found"
fi

echo ""
echo "=========================================="
echo "Build complete!"
echo "=========================================="
