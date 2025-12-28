# Building Blink's Thinks

This document provides instructions for building the project on Windows, Linux, and for the web from both platforms.

## Getting Started

First, clone the repository and initialize the raylib submodule:

```bash
git clone https://github.com/BlinkDynamo/blinks-thinks.git blinks-thinks
cd blinks-thinks/
git submodule update --init --recursive
```

## Platform-Specific Setup

### Windows

#### Prerequisites

**Enable Git Symlinks** (required for raylib headers)
1. Enable Developer Mode in Windows:
   - Settings -> Update & Security -> For developers -> Developer Mode
2. Configure Git to use symlinks and activate them:
   ```bash
   git config core.symlinks true
   git checkout HEAD -- include/raylib.h include/raymath.h include/rlgl.h
   ```

**Python**
- Download Python from https://www.python.org/downloads/
- During installation, check "Add Python to PATH"

**w64devkit**
- Download `w64devkit-x.xx.x.zip` from https://github.com/skeeto/w64devkit
- Extract to `c:\w64devkit`
- Always run `w64devkit.exe` as administrator when developing

**Emscripten SDK**
1. Open w64devkit as administrator.

2. Install and activate Emscripten:
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk.bat install latest
   ./emsdk.bat activate latest --system
   ./emsdk_env.bat
   ```

#### Building

1. Open w64devkit as administrator and navigate to the project root (blinks-thinks) if you are not already there.

2. Build the project:
   ```bash
   make native
   ```

This creates:
- `build/windows/debug/blinks_thinks.exe`
- `build/windows/release/blinks_thinks.exe`

If this is the first build, raylib will be downloaded automatically and added to `lib/`.

#### Running

From the w64devkit console:
```
./build/windows/release/blinks_thinks.exe
```

Or double-click the executable in File Explorer.

### Linux

#### Prerequisites

1. **Compiler**: clang with C++23 support
2. **Build dependencies** for raylib:
   - Debian/Ubuntu: `sudo apt install build-essential libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev`
   - Fedora: `sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel wayland-devel libxkbcommon-devel`
   - Arch: `sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama wayland libxkbcommon`

#### Building

```bash
make native    # Build native debug and release (builds raylib if needed)
```

This creates:
- `build/linux/debug/blinks_thinks`
- `build/linux/release/blinks_thinks`

On first build, raylib will be compiled from the submodule with Wayland and X11 support.

#### Running

```bash
./build/linux/release/blinks_thinks
```

### Web

Web builds work on Linux using Emscripten.

#### Prerequisites

1. **Emscripten SDK**
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Python 3**: For running the local web server

#### Building

1. Activate Emscripten environment (in each new terminal):

   On Windows:
   ```bash
   /path/to/emsdk/emsdk_env.bat
   ```

   On Linux/Mac:
   ```bash
   source /path/to/emsdk/emsdk_env.sh
   ```

2. Build the project:
   ```bash
   make web    # Build web debug and release (builds raylib if needed)
   ```

This creates:
- `build/web/debug/` - Debug build (unoptimized, larger file size)
- `build/web/release/` - Release build (optimized with -O3)

Each contains:
- `index.html`
- `index.js`
- `index.wasm`
- `index.data` (preloaded assets)

On first build, raylib will be compiled from the submodule.

#### Running

Start a local web server:
```bash
make serve
```

Then open http://localhost:8080 in your browser.

## Makefile Targets

- `make` or `make all`: Build both native and web (debug and release for each)
- `make native`: Build native debug and release (Windows/Linux detected automatically)
- `make web`: Build web debug and release (requires Emscripten)
- `make clean`: Remove all build artifacts and libraries
- `make serve`: Build web release and start local server
- `make info`: Display platform and compiler information

## Build Output Structure

```
build/
├── windows/
│   ├── debug/
│   │   └── blinks_thinks.exe
│   └── release/
│       └── blinks_thinks.exe
├── linux/
│   ├── debug/
│   │   └── blinks_thinks
│   └── release/
│       └── blinks_thinks
└── web/
    ├── debug/
    │   ├── index.html
    │   ├── index.js
    │   ├── index.wasm
    │   └── index.data
    └── release/
        ├── index.html
        ├── index.js
        ├── index.wasm
        └── index.data

lib/
├── windows/
│   └── libraylib.a (downloaded from GitHub release)
├── linux/
│   └── libraylib.a (built from external/raylib submodule)
└── web/
    └── libraylib_web.a (built from external/raylib submodule)

include/
├── raylib.h -> ../external/raylib/src/raylib.h (symlink)
├── raymath.h -> ../external/raylib/src/raymath.h (symlink)
├── rlgl.h -> ../external/raylib/src/rlgl.h (symlink)
└── *.hpp (game headers)

Raylib version: Determined by external/raylib submodule (currently 5.5)
```
