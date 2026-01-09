# Build and Release Script for The Assessor
# Automates PlatformIO build and renames output for M5Launcher

$VERSION = "0.1.0"
$PROJECT_NAME = "Assessor"
$OUTPUT_DIR = "dist"
$BUILD_PATH = ".pio/build/m5stack-cardputer/firmware.bin"

Write-Host "--- Starting Release Build for $PROJECT_NAME v$VERSION ---" -ForegroundColor Cyan

# 1. Clean and Build
Write-Host "[1/3] Building firmware..." -ForegroundColor Yellow
if (Get-Command "pio" -ErrorAction SilentlyContinue) {
    pio run
} else {
    Write-Host "Error: 'pio' command not found. Please run this script from the PlatformIO Terminal." -ForegroundColor Red
    exit 1
}

# 2. Check output
if (Test-Path $BUILD_PATH) {
    # Create dist directory if it doesn't exist
    if (!(Test-Path $OUTPUT_DIR)) {
        New-Item -ItemType Directory -Path $OUTPUT_DIR
    }

    # 3. Copy and Rename
    $RELEASE_NAME = "$($PROJECT_NAME)_v$($VERSION)_Alpha.bin"
    $DEST_PATH = "$OUTPUT_DIR/$RELEASE_NAME"
    
    Write-Host "[3/3] Packaging release: $RELEASE_NAME" -ForegroundColor Yellow
    Copy-Item -Path $BUILD_PATH -Destination $DEST_PATH
    
    Write-Host "Done! Your release is ready at: $DEST_PATH" -ForegroundColor Green
    Write-Host "Copy this file to your SD card for M5Launcher." -ForegroundColor Magenta
} else {
    Write-Host "Error: Build failed or firmware.bin not found at $BUILD_PATH" -ForegroundColor Red
    exit 1
}
