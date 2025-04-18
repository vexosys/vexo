#!/bin/bash

# Exit immediately if a command fails
set -e

# Check if a version number is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

VERSION=$1
PKG_NAME="hazeplayer-${VERSION}.pkg"
IDENTIFIER="com.haze.player"
INSTALL_PREFIX="/usr/local/haze"

# Create package root structure
echo "📂 Creating package structure..."
rm -rf package_root
mkdir -p package_root/${INSTALL_PREFIX}
mkdir -p package_root/usr/local/bin  # Ensure /usr/local/bin exists

# Copy files
echo "📄 Copying ffplay and skin directory..."
cp ./FFmpeg/ffplay package_root/${INSTALL_PREFIX}/hazeplayer
cp -r ./FFmpeg/skin package_root/${INSTALL_PREFIX}

# Set correct permissions
echo "🔧 Setting correct permissions..."
chmod -R 755 package_root/${INSTALL_PREFIX}/skin/
chmod -R 755 package_root/${INSTALL_PREFIX}/skin/Inter/

# Create a post-install script to set up symlink
echo "📝 Creating post-install script..."
mkdir -p package_scripts
cat <<EOF > package_scripts/postinstall
#!/bin/bash
set -e
ln -sf ${INSTALL_PREFIX}/hazeplayer /usr/local/bin/hazeplayer
EOF
chmod +x package_scripts/postinstall

# Build the package with post-install script
echo "📦 Building the package..."
pkgbuild --root package_root \
         --identifier "${IDENTIFIER}" \
         --version "${VERSION}" \
         --install-location "/" \
         --scripts package_scripts \
         "${PKG_NAME}"

# Cleanup
rm -rf package_root package_scripts

echo "✅ Package ${PKG_NAME} created successfully!"
echo "➡️  After installation, just type 'hazeplayer' in the terminal to run!"
