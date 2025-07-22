# OnionUI Dependencies Analysis

## Core System Dependencies

### Graphics & UI Libraries
- **SDL 1.2** (Primary UI Framework)
  - `libSDL` - Core graphics and input
  - `libSDL_image` - Image loading (PNG, JPEG)
  - `libSDL_ttf` - TrueType font rendering
  - `libSDL_rotozoom` - Image scaling/rotation
  - **Risk**: SDL 1.2 is end-of-life, no longer maintained

### Data Storage & Processing
- **SQLite3** - Embedded database
  - Game metadata storage
  - Play activity tracking
  - Configuration management
  - **Status**: Stable, actively maintained

- **cJSON** - JSON parsing library
  - Configuration file processing
  - API data handling
  - **Status**: Lightweight, stable dependency

- **libpng** - PNG image processing
  - Image format support
  - Texture loading for UI
  - **Status**: Standard, well-maintained

### System & Hardware Integration
- **Linux kernel APIs**
  - Framebuffer access
  - Input device handling
  - File system operations
  - **Risk**: Hardware-specific implementations

- **GNU C Library (glibc)**
  - Standard C runtime
  - POSIX API compliance
  - **Status**: Standard system dependency

### Build & Development Dependencies

#### Cross-Compilation Toolchain
- **ARM GCC Toolchain** (`arm-linux-gnueabihf-gcc`)
  - Cross-compiler for ARM target
  - **Version**: Buildroot-based custom toolchain
  - **Risk**: Custom toolchain maintenance overhead

#### Build System
- **GNU Make** - Build orchestration
- **Docker** - Development environment isolation
  - `aemiii91/miyoomini-toolchain:latest`
  - **Risk**: External Docker image dependency

#### Testing Framework
- **Google Test** (Limited usage)
  - Unit testing framework
  - **Status**: Minimal test coverage currently

## Third-Party Application Dependencies

### Major Integrations
- **RetroArch** (Emulation Framework)
  - **Version**: 1.20.0
  - **Components**: 100+ libretro cores
  - **Integration**: Patched version for Miyoo Mini
  - **Risk**: Large external dependency requiring coordination

- **DinguxCommander** (File Manager)
  - **Purpose**: File system navigation
  - **Status**: Community-maintained fork
  - **Risk**: Limited maintenance resources

- **Terminal Emulator**
  - **Purpose**: Command line access
  - **Status**: Custom implementation
  - **Risk**: Platform-specific implementation

### Community Packages
- **AdvanceMENU** - Alternative frontend
- **ScummVM** - Adventure game engine
- **Various emulator cores** - Game system support

## Documentation Dependencies

### Website Framework (Docusaurus)
- **@docusaurus/core**: `^2.4.3`
- **@docusaurus/preset-classic**: `^2.4.3`
- **React**: `^17.0.2`
- **TypeScript**: `^4.7.4`

### Additional Tools
- **Search**: `@easyops-cn/docusaurus-search-local`
- **Comments**: `@giscus/react`
- **Styling**: `sass`, `clsx`
- **Plugins**: Image zoom, SASS support

## Dependency Risk Assessment

### High Risk Dependencies
1. **SDL 1.2** - End-of-life, security concerns
2. **Custom ARM Toolchain** - Maintenance overhead
3. **RetroArch Integration** - Complex external dependency
4. **Docker Toolchain Image** - External build dependency

### Medium Risk Dependencies
1. **Hardware-specific drivers** - Platform lock-in
2. **Community third-party apps** - Limited maintenance
3. **Custom Linux patches** - Update complexity

### Low Risk Dependencies
1. **SQLite3** - Stable, widely used
2. **libpng** - Standard image library
3. **cJSON** - Lightweight, stable
4. **GNU Make** - Standard build tool

## Dependency Management Strategy

### Current Approach
- **Git Submodules** - Third-party source integration
- **Static Linking** - Reduced runtime dependencies
- **Custom Patches** - Hardware-specific modifications
- **Docker Isolation** - Build environment consistency

### Recommendations
1. **SDL Migration Plan** - Upgrade to SDL 2.x roadmap
2. **Dependency Auditing** - Regular security assessment
3. **Vendor Management** - Track third-party update cycles
4. **Alternative Toolchains** - Evaluate modern build systems
5. **Automated Updates** - CI/CD for dependency monitoring

## License Compliance
- **GPL/LGPL** - Primary licenses for core dependencies
- **MIT/BSD** - Permissive licenses for utilities
- **Proprietary** - Some emulator cores have restrictions
- **Documentation** - Community content licensing
