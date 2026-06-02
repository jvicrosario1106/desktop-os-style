# How to setup

## Git clone the repo

- `git clone <repository>`

## Install prerequisites

- For macOS,
  - `brew install cmake`
  - `brew install glfw`
  - To verify: `cmake --version`
- For Windows,
  - Install VS Build Tools ( C++ Workload )
  - CMake

Then install GLFW using vcpkg:

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat

.\vcpkg\vcpkg install glfw3:x64-windows
```

## Open in VS Code or VS or any IDE you prefer

- Install Extensions:
- C/C++
- CMake
- CMake Tools

## Configure CMake

- For MacOS + VSCode
- `Cmd + Shift + P`
- Then choose, `CMake: Configure`
- Select Compiler, either Apple Clang or MSVC
  - Note: Not sure how to configure CMake in other IDlike Visual StudioE, so i leave it you guys to figure out

## Build

- MacOS + VSCode
- `Cmd + Shift + P` again
- Then choose, `CMake: Build`
- Then go to build folder if already existing after `CMake:Build`
- or `create build folder` -> `cd build` -> `cmake ..` -> `cmake --build .`
  - Note: Not sure how to build in other IDE like Visual Studio, so i leave it you guys to figure out

## Run

- Make sure the terminal is inside build directory
- then do `./OSEmulator` or `./OSEmulators.exe`
