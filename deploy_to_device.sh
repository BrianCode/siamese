#!/bin/bash
# Script to deploy and run a specified binary on a device using ADB

# Check if ADB is available
if ! command -v adb &> /dev/null; then
    echo "Error: ADB is not installed or not in PATH"
    echo "Please install ADB and make sure it's in your PATH"
    exit 1
fi

# Check if a binary is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <binary>"
    exit 1
fi

BINARY=$1

# Check if the binary exists locally
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary '$BINARY' not found. Make sure it exists."
    exit 1
fi

# Check if a device is connected
if ! adb devices | grep -q "device$"; then
    echo "Error: No device connected or device not authorized"
    echo "Please connect a device and authorize ADB access"
    exit 1
fi

# Create directory on device if it doesn't exist
adb shell "mkdir -p /data/local/tmp"

# Check if the binary exists on the device
if adb shell "[ -f /data/local/tmp/$BINARY ]"; then
    echo "Binary exists on device. Removing it..."
    adb shell "rm /data/local/tmp/$BINARY"
fi

# Push the binary to the device
echo "Pushing $BINARY to device..."
adb push "$BINARY" /data/local/tmp/
if [ $? -ne 0 ]; then
    echo "Error: Failed to push $BINARY to device"
    exit 1
fi

# Set execute permissions
adb shell "chmod +x /data/local/tmp/$BINARY"

# Run the binary
echo "Running $BINARY on device..."
adb shell "/data/local/tmp/$BINARY"