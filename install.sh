#!/bin/bash
set -e

SRC_DIR="include"

if [ ! -d "$SRC_DIR" ]; then
    echo "Install error: directory $SRC_DIR not found"
    exit 1
fi

if [ "$EUID" -ne 0 ]; then
    echo "Please run this script with sudo or as root"
    exit 1
fi
COMPILER_PATHS=(
    "/usr/local/include"
    "/usr/include"
)

echo "installing headers to /usr/local/include..."

for INSTALL_DIR in "${COMPILER_PATHS[@]}"; do
    if [ -d "$INSTALL_DIR" ]; then
        echo "installing to root of $INSTALL_DIR..."
        cp -R "$SRC_DIR"/* "$INSTALL_DIR/"
        cd "$SRC_DIR"
        for item in *; do
            if [ -d "$INSTALL_DIR/$item" ]; then
                find "$INSTALL_DIR/$item" -type d -exec chmod 755 {} +
                find "$INSTALL_DIR/$item" -type f -exec chmod 644 {} +
            elif [ -f "$INSTALL_DIR/$item" ]; then
                chmod 644 "$INSTALL_DIR/$item"
            fi
        done
        cd - > /dev/null
    fi
done

echo "CymboliC has been successfully installed!"
