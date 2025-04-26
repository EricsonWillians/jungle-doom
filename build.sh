#!/bin/bash

# Enhanced Build Script for Jungle Doom
# Provides safer build operations with advanced logging and error handling.

# Exit immediately if a command exits with a non-zero status
set -e

# Variables
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
LOG_FILE="$SCRIPT_DIR/build.log"
CLEAN_BUILD=false

# ANSI Color Codes
COLOR_RESET="\033[0m"
COLOR_RED="\033[0;31m"
COLOR_GREEN="\033[0;32m"
COLOR_YELLOW="\033[1;33m"
COLOR_BLUE="\033[0;34m"

# Logging Functions
log() {
    local LEVEL=$1
    local MESSAGE=$2
    local COLOR

    case "$LEVEL" in
        INFO)
            COLOR=$COLOR_BLUE
            ;;
        SUCCESS)
            COLOR=$COLOR_GREEN
            ;;
        WARNING)
            COLOR=$COLOR_YELLOW
            ;;
        ERROR)
            COLOR=$COLOR_RED
            ;;
        *)
            COLOR=$COLOR_RESET
            ;;
    esac

    TIMESTAMP=$(date +"%Y-%m-%d %H:%M:%S")
    echo -e "${COLOR}[$TIMESTAMP] [$LEVEL] $MESSAGE${COLOR_RESET}" | tee -a "$LOG_FILE"
}

# Function to check dependencies
check_dependencies() {
    log "INFO" "Checking for required dependencies..."

    local dependencies=("cmake" "make" "git")
    for cmd in "${dependencies[@]}"; do
        if ! command -v "$cmd" >/dev/null 2>&1; then
            log "ERROR" "Required command '$cmd' is not installed. Aborting."
            exit 1
        else
            log "INFO" "Found dependency: $cmd"
        fi
    done

    log "SUCCESS" "All dependencies are satisfied."
}

# Function to parse command-line arguments
parse_args() {
    while [[ "$#" -gt 0 ]]; do
        case $1 in
            --clean)
                CLEAN_BUILD=true
                ;;
            --help|-h)
                echo "Usage: ./build.sh [--clean] [--help|-h]"
                echo "  --clean    Perform a clean build without confirmation prompts."
                echo "  --help, -h Display this help message."
                exit 0
                ;;
            *)
                log "WARNING" "Unknown option: $1"
                ;;
        esac
        shift
    done
}

# Function to perform a clean build
clean_build_directory() {
    if [ "$CLEAN_BUILD" = true ]; then
        log "INFO" "Clean build option detected. Cleaning build directory."
    else
        read -p "Are you sure you want to clean the build directory ($BUILD_DIR)? [y/N]: " confirm
        if [[ "$confirm" =~ ^[Yy]$ ]]; then
            log "INFO" "User confirmed. Cleaning build directory."
        else
            log "INFO" "User declined. Skipping cleaning of build directory."
            return
        fi
    fi

    if [ -d "$BUILD_DIR" ]; then
        # Ensure BUILD_DIR is not root or empty
        if [[ "$BUILD_DIR" == "/" ]] || [[ -z "$BUILD_DIR" ]]; then
            log "ERROR" "Build directory is root or empty. Aborting to prevent data loss."
            exit 1
        fi
        rm -rf "${BUILD_DIR:?}/"*
        log "SUCCESS" "Build directory cleaned."
    else
        log "INFO" "Build directory does not exist. Creating $BUILD_DIR."
        mkdir -p "$BUILD_DIR"
    fi
}

# Function to run CMake
run_cmake() {
    log "INFO" "Running CMake..."
    cmake .. | tee -a "$LOG_FILE"
    log "SUCCESS" "CMake configuration completed."
}

# Function to build the project
build_project() {
    CPU_CORES=$(nproc)
    log "INFO" "Starting build with $CPU_CORES parallel jobs."
    make -j"$CPU_CORES" | tee -a "$LOG_FILE"
    log "SUCCESS" "Build completed successfully."
}

# Function to initialize the build process
initialize_build() {
    log "INFO" "Initializing build process for Jungle Doom."
    
    # Create build directory if it doesn't exist
    if [ ! -d "$BUILD_DIR" ]; then
        log "INFO" "Build directory does not exist. Creating $BUILD_DIR."
        mkdir -p "$BUILD_DIR"
        log "SUCCESS" "Build directory created."
    else
        log "INFO" "Build directory exists at $BUILD_DIR."
    fi
}

# Function to finalize the build process
finalize_build() {
    log "SUCCESS" "Build process completed successfully. Logs are available at $LOG_FILE."
}

# Trap to handle unexpected errors
trap 'log "ERROR" "An unexpected error occurred. Check the log file for details."; exit 1' ERR

# Start of the script
echo "" > "$LOG_FILE"  # Clear previous log
log "INFO" "=================== Build Started ==================="
parse_args "$@"
check_dependencies
initialize_build
clean_build_directory

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake and build
run_cmake
build_project

# Finalize
finalize_build
