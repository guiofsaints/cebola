# OnionUI Project Structure Analysis

## Architecture Pattern
**Modular Embedded System Architecture**

The OnionUI project follows a modular embedded systems architecture with clear separation of concerns:

### Core System Layers

#### 1. Hardware Abstraction Layer (HAL)
- **Location**: `src/common/system/`
- **Components**: Battery management, input handling, display management
- **Purpose**: Abstract hardware-specific functionality for Miyoo Mini/Mini+

#### 2. System Services Layer
- **Location**: `src/axp/`, `src/batmon/`, `src/keymon/`
- **Components**: 
  - Power management (AXP chip interface)
  - Battery monitoring
  - Key input monitoring
  - System state management

#### 3. UI Framework Layer
- **Location**: `src/common/theme/`, `src/common/components/`
- **Components**:
  - Theme rendering system
  - UI component library
  - SDL-based graphics abstraction

#### 4. Application Layer
- **Location**: Individual `src/*/` directories
- **Major Applications**:
  - `gameSwitcher/` - Game switching interface
  - `packageManager/` - App/emulator package management
  - `tweaks/` - System settings interface
  - `themeSwitcher/` - Theme management
  - `infoPanel/` - Information display overlay

### Module Dependencies

```
Applications Layer
    ↓
UI Framework (SDL, Theme System)
    ↓
System Services (Battery, Input, State)
    ↓
Hardware Abstraction Layer
    ↓
Linux Kernel + Miyoo Hardware
```

## Directory Structure Analysis

### Source Code Organization (`src/`)
- **Modular Design**: Each major feature is a separate directory with its own Makefile
- **Shared Components**: Common utilities in `src/common/`
- **Clear Separation**: UI components separate from system services

### Build System (`Makefile`, `src/common/`)
- **Hierarchical Makefiles**: Main Makefile orchestrates module builds
- **Cross-Compilation**: ARM-specific build configuration
- **Docker Integration**: Containerized build environment

### Third-Party Integration (`third-party/`)
- **RetroArch**: Major emulation framework
- **DinguxCommander**: File manager
- **Terminal**: Terminal emulator
- **SearchFilter**: Game search functionality

### Static Assets (`static/`)
- **Pre-built Components**: Static configurations and packages
- **Package System**: Modular app/emulator packages
- **Configuration Management**: System configuration templates

### Documentation (`website/`)
- **Docusaurus-based**: Modern documentation framework
- **Versioned Docs**: Multiple version support
- **Community Focus**: User and developer documentation

## Architectural Strengths

1. **Modularity**: Clear module boundaries enable independent development
2. **Reusability**: Common utilities shared across modules
3. **Maintainability**: Well-organized directory structure
4. **Extensibility**: Plugin-style app/emulator packages
5. **Cross-Platform Build**: Docker-based development workflow

## Architectural Considerations

1. **Tight Hardware Coupling**: Architecture specific to Miyoo Mini platform
2. **C-based Development**: Lower-level development complexity
3. **Resource Constraints**: Must optimize for limited embedded resources
4. **Build Complexity**: Multi-stage build process with cross-compilation
