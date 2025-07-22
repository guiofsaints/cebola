# OnionUI Entities and Data Structures Analysis

## Core Data Entities

### System Entities

#### 1. Device Model (`src/common/system/device_model.h`)
```c
typedef struct {
    int DEVICE_ID;      // MIYOO283 or MIYOO354
    char DEVICE_SN[13]; // Device serial number
} DeviceModel;
```
**Purpose**: Hardware identification and platform-specific behavior

#### 2. System Settings (`src/common/system/settings.h`)
```c
typedef struct {
    int volume;
    char keymap[JSON_STRING_LEN];
    int mute;
    bool bgm_mute;
    int bgm_volume;
    int brightness;
    char language[JSON_STRING_LEN];
    int sleep_timer;
    // ... 40+ configuration fields
} Settings;
```
**Purpose**: Global system configuration and user preferences

### UI Framework Entities

#### 3. List Component (`src/common/components/list.h`)
```c
typedef enum list_type { LIST_SMALL, LIST_LARGE } ListType;
typedef enum item_type { ACTION, TOGGLE, MULTIVALUE } ListItemType;

typedef struct ListItem {
    int _id;
    ListItemType item_type;
    bool disabled;
    char label[STR_MAX];
    char description[STR_MAX];
    char payload[STR_MAX];
    void *payload_ptr;
    int value;
    int value_min;
    int value_max;
    char value_labels[MAX_NUM_VALUES][STR_MAX];
    void (*action)(void *self);
    // ... more fields
} ListItem;

typedef struct List {
    char title[STR_MAX];
    int _id;
    int item_count;
    int active_pos;
    int scroll_pos;
    ListType list_type;
    ListItem *items;
    bool has_sticky;
    bool _created;
} List;
```
**Purpose**: Generic UI list component for menus and selection interfaces

#### 4. Theme System (`src/common/theme/config.h`)
```c
typedef enum TextAlign { LEFT, CENTER, RIGHT } Theme_TextAlign;

typedef struct Theme_BatteryPercentage {
    bool visible;
    char font[STR_MAX];
    int size;
    SDL_Color color;
    int offsetX;
    int offsetY;
    Theme_TextAlign textAlign;
    bool fixed;
} BatteryPercentage_s;

typedef struct Theme_Frame {
    int border_left;
    int border_right;
} Frame_s;

typedef struct Theme_FontStyle {
    char font[STR_MAX];
    int size;
    SDL_Color color;
} FontStyle_s;
```
**Purpose**: Theme configuration and visual styling

#### 5. Theme Resources (`src/common/theme/resources.h`)
```c
typedef enum theme_images {
    NULL_IMAGE,
    BACKGROUND,
    GRID_BG,
    PREVIEW_BG,
    BRIGHTNESS_0,
    // ... 30+ image types
    images_count
} ThemeImages;

typedef enum theme_fonts {
    NULL_FONT,
    TITLE,
    HINT,
    GRID1x4,
    GRID3x4,
    LIST,
    BATTERY,
    fonts_count
} ThemeFonts;

typedef struct Theme_Resources {
    Theme_s theme;
    Theme_s theme_back;
    bool _theme_loaded;
    SDL_Surface *surfaces[(int)images_count];
    TTF_Font *fonts[(int)fonts_count];
} Theme_Resources;
```
**Purpose**: Resource management for themes and visual assets

### Game Management Entities

#### 6. Game History (`src/gameSwitcher/gs_model.h`)
```c
typedef struct {
    char label[STR_MAX * 2];
    char rompath[STR_MAX * 2];
    char imgpath[STR_MAX * 2];
    char launch[STR_MAX * 2];
    int type;
    int lineNo;
} RecentItem;

typedef struct {
    RecentItem recentItem;
    SDL_Surface *romScreen;
    char rom_name[STR_MAX * 2];
    char name[STR_MAX * 2];
    char shortname[STR_MAX * 2];
    char core_name[STR_MAX * 2];
    char core_path[STR_MAX * 2];
    char totalTime[100];
    int index;
    bool processed;
    bool is_running;
} Game_s;
```
**Purpose**: Game history and metadata management

#### 7. JSON Game Entry (`src/common/components/JsonGameEntry.h`)
```c
typedef struct json_game_entry_s {
    char label[STR_MAX];
    char launch[STR_MAX];
    int type;
    char rompath[STR_MAX];
    char imgpath[STR_MAX];
    char emupath[STR_MAX];
} JsonGameEntry;
```
**Purpose**: Serializable game metadata for favorites and configuration

### Package Management Entities

#### 8. Package System (`src/packageManager/globals.h`)
```c
typedef struct Package {
    char name[STR_MAX];
    char label[STR_MAX];
    char description[STR_MAX];
    bool installed;
    bool changed;
    bool complete;
    bool has_roms;
    // ... additional fields
} Package;

static Package packages[4][LAYER_ITEM_COUNT];
static int package_count[] = {0, 0, 0, 0};
static int package_installed_count[] = {0, 0, 0, 0};
```
**Purpose**: Application and emulator package management

### State Management Entities

#### 9. Application State (`src/gameSwitcher/gs_appState.h`)
```c
typedef struct {
    bool quit;
    bool exit_to_menu;
    bool changed;
    bool current_game_changed;
    bool brightness_changed;
    bool pop_menu_open;
    bool show_time;
    bool show_total;
    bool show_legend;
    bool is_overlay;
    int view_mode;
    int view_restore;
    int current_game;
    // ... more state fields
    SDL_Surface *surfaceGameName;
    SDL_Rect game_name_size;
    int game_name_max_width;
} AppState;
```
**Purpose**: Runtime application state management

### System State Entities

#### 10. Main UI State (`src/common/system/state.h`)
```c
typedef enum mainui_states {
    MAIN_MENU,
    RECENTS,
    FAVORITES,
    GAMES,
    EXPERT,
    APPS
} MainUIState;
```
**Purpose**: Navigation state management for main interface

## Data Structure Patterns

### Memory Management Patterns
- **Static Arrays**: Fixed-size arrays for game lists and packages
- **Dynamic Allocation**: SDL surfaces and temporary data structures
- **Resource Pools**: Pre-allocated theme resources and fonts

### String Handling Patterns
- **Fixed Buffers**: `STR_MAX` (typically 256-512 bytes) for most strings
- **Path Buffers**: Larger buffers for file paths (2-4KB)
- **JSON Integration**: cJSON library for configuration persistence

### State Management Patterns
- **Singleton Globals**: System-wide state in static variables
- **Flag-based State**: Boolean flags for UI state tracking
- **Enum-driven Logic**: Type-safe state machines using enums

### UI Component Patterns
- **Component Hierarchies**: Nested structures for complex UI elements
- **Event-driven Updates**: Flag-based change detection
- **Resource Binding**: Direct pointers to SDL surfaces and fonts

## Entity Relationships

### Core Dependencies
```
Settings ──→ Theme_Resources ──→ SDL_Surface
    │              │
    └──→ Language ──┘
    
Game_s ──→ RecentItem ──→ JsonGameEntry
    │          │              │
    └──→ ROM ──┘              └──→ cJSON
    
Package ──→ Installation ──→ FileSystem
    │           │                │
    └──→ Layer ──┘               └──→ Script
```

### Data Flow Patterns
1. **Configuration Loading**: Settings → JSON → UI State
2. **Game Management**: ROM Files → Cache DB → Game_s → UI Display
3. **Theme Application**: Theme Config → Resources → SDL Surfaces
4. **Package Installation**: Package Metadata → Scripts → File Operations

## Architecture Insights

### Strengths
- **Type Safety**: Extensive use of enums for state management
- **Resource Management**: Clear separation of data and presentation
- **Modularity**: Well-defined entity boundaries between components
- **Performance**: Static allocation for frequently accessed data

### Areas for Improvement
- **Memory Safety**: Limited bounds checking on string operations
- **Data Validation**: Minimal input validation for parsed data
- **Error Handling**: Inconsistent error propagation patterns
- **Documentation**: Missing formal entity relationship documentation
