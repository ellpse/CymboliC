#!/bin/bash
set -e

SRC_DIR="include/cymbolic"
INSTALL_DIR="/usr/local/include/cymbolic"
if [ ! -d "$SRC_DIR" ]; then
    echo "install error: directory $SRC_DIR not found"
    exit 1
fi

if [ "$EUID" -ne 0 ]; then
    echo "please run this script with sudo or as root"
    exit 1
fi

mkdir -p "/usr/local/include"

echo "installing headers to $INSTALL_DIR..."
cp -R "$SRC_DIR" "/usr/local/include/"

find "$INSTALL_DIR" -type d -exec chmod 755 {} +
find "$INSTALL_DIR" -type f -exec chmod 644 {} +

echo "CymboliC has been installed to /usr/local/include/cymbolic/CymboliC.h"
