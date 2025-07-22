# OnionUI Technology Stack Analysis

## Core Technology Stack

### Programming Languages
- **Primary: C (95%)**
  - System-level programming for embedded environment
  - SDL-based UI development
  - Low-level hardware abstraction
  - Memory-efficient implementations

- **Secondary: C++ (5%)**
  - Limited use in specific modules (libgamename)
  - Object-oriented components where beneficial

- **Build Scripts: Shell/Make**
  - Bash scripts for build automation
  - Complex Makefile-based build system

### Core Libraries & Frameworks

#### Graphics & UI
- **SDL 1.2** - Primary graphics and input library
  - SDL_image - Image loading and manipulation
  - SDL_ttf - TrueType font rendering
  - SDL_rotozoom - Image scaling and rotation

#### Data Management
- **SQLite3** - Database for game metadata and activity tracking
- **cJSON** - JSON parsing for configuration files
- **libpng** - PNG image processing

#### System Integration
- **Linux kernel APIs** - Direct system calls for hardware access
- **GNU toolchain** - GCC cross-compiler for ARM target
- **Custom drivers** - Hardware-specific interfaces (AXP power management)

### Development Environment

#### Build System
- **GNU Make** - Primary build orchestration
- **Cross-compilation toolchain**: `arm-linux-gnueabihf-gcc`
- **Docker containerization** - `aemiii91/miyoomini-toolchain:latest`
- **Target Architecture**: ARM Cortex-A7 with NEON SIMD

#### Toolchain Configuration
```c
CFLAGS := -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 
         -mfloat-abi=hard -march=armv7ve
```

### Third-Party Integrations

#### Emulation Framework
- **RetroArch** - Complete emulation framework
  - 100+ libretro cores
  - Save state management
  - Input remapping
  - Shader support

#### Community Applications
- **DinguxCommander** - File management
- **Terminal emulator** - Command line access
- **SearchFilter** - Game search functionality

### Documentation Stack
- **Docusaurus v2.4.3** - Static site generator
- **React 17** - UI framework for documentation
- **TypeScript 4.7** - Type-safe development
- **Markdown/MDX** - Content authoring

### Platform Specifics

#### Target Hardware
- **Miyoo Mini/Mini+** handheld devices
- **ARM Cortex-A7** processor
- **Linux-based** custom firmware
- **Limited resources** - Optimized for embedded constraints

#### Runtime Environment
- **Embedded Linux** - Custom firmware layer
- **Framebuffer graphics** - Direct hardware access
- **Custom init system** - Specialized boot process

## Technology Assessment

### Strengths
1. **Performance Optimized** - ARM-specific optimizations with NEON
2. **Resource Efficient** - C-based development for minimal overhead
3. **Hardware Integrated** - Direct hardware abstraction
4. **Proven Technologies** - Mature, stable library choices
5. **Cross-Platform Development** - Docker-based workflow

### Challenges
1. **Legacy Dependencies** - SDL 1.2 (end-of-life)
2. **Build Complexity** - Complex cross-compilation setup
3. **Hardware Lock-in** - ARM/Miyoo-specific optimizations
4. **Limited Debugging** - Embedded development constraints
5. **Toolchain Maintenance** - Custom Docker toolchain dependency

### Modernization Opportunities
1. **SDL Migration** - Consider upgrading to SDL 2.x
2. **Build System** - Evaluate CMake or modern build tools
3. **Testing Framework** - Add automated testing infrastructure
4. **CI/CD Pipeline** - Enhance GitHub Actions workflows
5. **Package Management** - Consider modern C package managers
