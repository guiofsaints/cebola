# OnionUI Setup and Development Guide

## Project Overview

OnionUI is an enhanced operating system for Miyoo Mini and Mini+ handheld gaming devices, featuring fine-tuned emulation with 100+ built-in emulators, auto-save functionality, extensive customization options, and a wealth of features for retro gaming.

## Prerequisites

### System Requirements
- **Operating System**: Linux (recommended), or Linux VM with Docker support
- **Docker**: Required for cross-compilation toolchain
- **Git**: For version control and submodule management
- **Available Storage**: ~2-5GB for build artifacts and dependencies

### Recommended Development Environment
- **Linux VM Options**: VMware Workstation Player or VirtualBox
- **Pre-installed Linux Images**: Available at [linuxvmimages.com](https://www.linuxvmimages.com/)
- **Docker Image**: Based on [Shaun Inman's union-miyoomini-toolchain](https://github.com/shauninman/union-miyoomini-toolchain)

## Installation and Setup

### 1. Repository Setup
```bash
# Clone the repository
git clone https://github.com/OnionUI/Onion.git
cd Onion/

# Initialize and update git submodules
make git-submodules
```

### 2. Docker Toolchain Setup
The project uses a Docker-based cross-compilation environment:

```bash
# Pull the required Docker toolchain image
docker pull aemiii91/miyoomini-toolchain:latest

# Or use the Makefile target (automatic)
make with-toolchain
```

### 3. Development Build
```bash
# Standard build (release configuration)
make with-toolchain

# Development build (with debug logging)
make with-toolchain CMD=dev

# Address sanitizer build (for debugging)
make with-toolchain CMD=asan
```

### 4. Interactive Toolchain Access
```bash
# Access the Docker toolchain interactively
make toolchain

# Inside the container, you can run:
# make dev     - Development build with debug symbols
# make asan    - Address sanitizer build
# make clean   - Clean build artifacts
```

## Build System Architecture

### Core Build Targets

#### Main Build Process
```bash
make all        # Complete build pipeline (equivalent to 'make dist')
make dist       # Create distribution packages
make build      # Build core, apps, and external components
make core       # Build OnionUI core binaries
make apps       # Build application packages
make external   # Build third-party components (RetroArch, etc.)
```

#### Development Targets
```bash
make dev        # Development build with debug symbols
make asan       # Address sanitizer build for memory debugging
make test       # Run unit tests (limited coverage currently)
make clean      # Clean build artifacts
make deepclean  # Deep clean including cached dependencies
```

### Build Directory Structure
```
build/                          # Main build output
├── .tmp_update/               # Core system files
│   ├── bin/                   # Compiled binaries
│   ├── lib/                   # Shared libraries
│   ├── res/                   # Resources (images, fonts)
│   └── script/                # System scripts
├── App/                       # Application packages
│   └── PackageManager/        # Package management system
├── RetroArch/                 # RetroArch emulation core
└── miyoo/                     # Hardware-specific files

dist/                          # Distribution packages
├── miyoo/                     # Complete firmware image
└── RetroArch/                 # Separate RetroArch package

release/                       # Final release archives
```

## Component Build Process

### Core OnionUI Components
The build process compiles multiple specialized components:

```bash
# UI and System Components
bootScreen          # Boot splash screen
chargingState       # Battery charging interface
gameSwitcher        # Game switching interface
mainUiBatPerc       # Main UI battery percentage
themeSwitcher       # Theme management
tweaks              # System settings and tweaks

# System Services
batmon              # Battery monitoring daemon
playActivity        # Game play tracking
keymon              # Key input monitoring
axp                 # Power management

# Utilities
packageManager      # App installation system
infoPanel           # Information display panel
prompt              # System prompts and dialogs
setState            # System state management
```

### External Dependencies Build
```bash
# RetroArch (Emulation Core)
make external  # Builds RetroArch with Onion-specific patches

# Third-party Applications
SearchFilter        # Game search and filtering
Terminal           # Terminal emulator
DinguxCommander    # File manager
```

### Library Dependencies
The build system automatically handles:
- **SDL 1.2 Suite**: Graphics, input, audio, image, and TTF support
- **SQLite3**: Database for game metadata and settings
- **cJSON**: Configuration file parsing
- **libpng**: Image processing
- **Custom Libraries**: Hardware-specific implementations

## Cross-Compilation Details

### Toolchain Configuration
- **Target Architecture**: ARM Cortex-A7 (armv7l)
- **Cross-Compiler**: `arm-linux-gnueabihf-gcc`
- **C Standard**: GNU18 (C18 with GNU extensions)
- **C++ Standard**: C++17
- **Optimization**: `-Ofast -g0` for release, `-g3` for debug

### Platform-Specific Build Flags
```makefile
# Core compilation flags
CFLAGS := -I../../include -I../common 
CFLAGS += -DPLATFORM_MIYOOMINI -DONION_VERSION="\"$(VERSION)\""
CFLAGS += -Wall -fno-omit-frame-pointer

# Debug builds
DEBUG=1: CFLAGS += -DLOG_DEBUG -g3

# Sanitizer builds  
SANITIZE=1: CFLAGS += -fno-omit-frame-pointer -fsanitize=address
```

## Testing and Quality Assurance

### Available Testing
```bash
# Unit tests (limited coverage)
make test

# Static analysis
make static-analysis  # Uses cppcheck

# Memory debugging
make asan            # Address sanitizer build
```

### Code Quality Tools
- **cppcheck**: Static analysis for C/C++ code
- **AddressSanitizer**: Runtime memory error detection
- **Google Test**: Unit testing framework (minimal usage currently)

## Installation and Deployment

### First Installation
1. Build the project: `make with-toolchain`
2. Copy files from `dist/` directory to SD card
3. Insert SD card into Miyoo Mini device

### Development Updates
For testing subsequent builds after initial installation:
1. Build: `make with-toolchain`
2. Copy files from `build/.tmp_update/` to device
3. Restart device

### Package Structure
- **Full Installation**: Complete firmware replacement from `dist/`
- **Update Package**: Incremental updates from `build/.tmp_update/`
- **Configuration**: Settings and themes in packaged format

## Development Workflow

### Typical Development Cycle
```bash
# 1. Set up development environment
make git-submodules
make toolchain      # Interactive shell for development

# 2. Inside toolchain container
make dev           # Development build
make clean         # Clean previous build
make asan          # Memory debugging build

# 3. Deploy and test
# Copy build/.tmp_update/ to device
# Test functionality on hardware

# 4. Code quality checks
make static-analysis
cppcheck -I include --enable=all src/
```

### Debugging Support
- **Debug Builds**: Enable with `DEBUG=1` or `make dev`
- **Debug Logging**: Comprehensive logging system with `LOG_DEBUG`
- **AddressSanitizer**: Memory error detection with `make asan`
- **GDB Support**: Debug symbols available in debug builds

## Configuration Management

### Build Configuration
- **Version Management**: Automatic version injection via Makefile
- **Platform Detection**: Automatic Miyoo Mini/Mini+ detection
- **Feature Flags**: Compile-time feature selection
- **Resource Packaging**: Automatic asset bundling and compression

### Environment Variables
```bash
VERSION_OVERRIDE    # Override default version
UNION_PLATFORM      # Target platform selection
GTEST_INCLUDE_DIR   # Google Test headers location
COLORS=1            # Enable colored build output
```

## Common Issues and Troubleshooting

### Docker-related Issues
- **Permission Issues**: Ensure Docker daemon is running and user has Docker permissions
- **Image Pull Failures**: Check internet connectivity and Docker registry access
- **Mount Issues**: Verify volume mounting in `make toolchain` command

### Build Issues
- **Missing Dependencies**: Run `make git-submodules` to ensure all submodules are initialized
- **Cross-compilation Errors**: Verify toolchain image is properly pulled and accessible
- **Memory Issues**: Address sanitizer builds require more memory and disk space

### Hardware-specific Issues
- **Device Compatibility**: Ensure targeting correct Miyoo Mini variant (original vs Plus)
- **SD Card Compatibility**: Use high-quality SD cards for reliable operation
- **Power Management**: Some features require specific power management configurations

## Advanced Development

### Custom Builds
- **Feature Customization**: Modify component makefiles to enable/disable features
- **Resource Customization**: Replace assets in respective `res/` directories
- **Theme Development**: Create custom themes using theme specification

### Contributing
- **Code Style**: Follow existing C/C++ conventions in the codebase
- **Testing**: Add unit tests for new functionality when possible
- **Documentation**: Update relevant documentation for new features
- **Platform Support**: Ensure changes work on both Miyoo Mini and Mini+ devices

## Resources and Documentation

### Official Documentation
- **Website**: [onionui.github.io/docs](https://onionui.github.io/docs)
- **Features Guide**: [onionui.github.io/docs/features](https://onionui.github.io/docs/features)
- **FAQ**: [onionui.github.io/docs/faq](https://onionui.github.io/docs/faq)

### Community Resources
- **Themes**: [GitHub OnionUI/Themes](https://github.com/OnionUI/Themes)
- **Ports**: [onionui.github.io/docs/ports](https://onionui.github.io/docs/ports)
- **Development Guides**: Available in website documentation

### Technical References
- **Toolchain**: [union-miyoomini-toolchain](https://github.com/shauninman/union-miyoomini-toolchain)
- **Hardware Specs**: Miyoo Mini ARM Cortex-A7 handheld gaming device
- **Emulation**: RetroArch-based multi-system emulation platform
