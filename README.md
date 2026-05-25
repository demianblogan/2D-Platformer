# 2D Platformer

A 2D platformer built in C++23 with SFML 3 and a custom Entity-Component-System.

## Status

In active development.

## Tech

- C++23
- SFML 3
- Custom ECS (sparse-set)
- Data-driven design (JSON via nlohmann/json)
- CMake + vcpkg

## Requirements

- A C++23 compiler (Visual Studio 2026 or a recent MSVC/Clang/GCC)
- [CMake](https://cmake.org/) 3.25 or newer
- [vcpkg](https://github.com/microsoft/vcpkg)

Dependencies (SFML 3, nlohmann/json) are declared in `vcpkg.json` and fetched
automatically by vcpkg during configuration.

## Build

1. Install vcpkg and note its location, e.g. `C:/dev/vcpkg`.

2. Create your local preset file. Copy `CMakeUserPresets.json.example` to
   `CMakeUserPresets.json` and replace the toolchain path with the path to your
   own vcpkg installation:

```json
   "CMAKE_TOOLCHAIN_FILE": "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

   This file is local to your machine and is not tracked by Git.

3. Open the project folder in Visual Studio (File > Open > Folder). It will
   detect the CMake project and configure using the `Default (vcpkg)` preset.
   The first configuration builds the dependencies from source and may take
   several minutes.

4. Build the `Platformer` target.

## Running

The game loads assets with paths relative to the working directory, so it must
be launched from the project root (where the `assets` folder lives).

- In Visual Studio, the launch working directory is set to the project root.
- When running the built executable directly, place a copy of the `assets`
  folder next to it.

## License

[MIT](LICENSE)