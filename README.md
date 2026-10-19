# Underground Test Console — Geode source project

Targets Geometry Dash for Windows x64. The project recreates the visible console and F6 stage cycling described in the video transcript.

## Implemented in source
- In-level console overlay.
- Configurable `Cycle test mode` keybind (default F6).
- `OFF` (0 display ticks), `STAGE 1` (2 display ticks), `STAGE 2` (4 display ticks).
- Logs mode changes to the Geode log.

## Important limitation
This is a display-only prototype. It does **not** implement the original mod's actual frame-saving/timing behaviour. The transcript does not describe the internal algorithm, so it has not been guessed.

## Build locally (Windows x64)
1. Install the Geode CLI and SDK using the official docs: https://docs.geode-sdk.org/getting-started/sdk/
2. Install the required C++ tools (Clang or Visual Studio toolchain, CMake, Git) following: https://docs.geode-sdk.org/getting-started/ide-setup/
3. Open a terminal in this project folder and run `geode build` (or configure/build with the IDE).
4. The Geode CLI should produce a `.geode` package in the build output when configuration succeeds.

## Build through GitHub Actions
A workflow is included at `.github/workflows/build.yml`. Push this project to a GitHub repository, then open **Actions → Build Geode mod → Run workflow**. Download the `underground-geode-build-output` artifact from the completed run and check whether it contains a `.geode` file. A successful build is required; the source ZIP itself is not an installable mod.

## Files
- `mod.json` — mod metadata and keybind setting.
- `CMakeLists.txt` — CMake/Geode project definition.
- `src/main.cpp` — console overlay and mode cycling.
- `.github/workflows/build.yml` — Windows CI build workflow.
