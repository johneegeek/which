# Building from Source

This document describes how to build the `which` command from source on Windows.

## Prerequisites

Before building, ensure you have the following installed:

1. **[Boost C++ Libraries](https://www.boost.org/)** - Required for string algorithms, filesystem, and range utilities
   - Download the pre-built binaries for Windows
   - Set the `BOOST_ROOT` environment variable to the Boost installation directory

2. **C++ Compiler** - One of:
   - Visual Studio 2017 or later (MSVC)
   - MinGW-w64
   - Clang for Windows

3. **[CMake](https://cmake.org/)** - Version 3.24 or later
   - Make sure it's added to your PATH

4. **[Ninja](https://ninja-build.org/)** - Build system
   - Download and add to your PATH

## Quick Start

If you have `make` available (via Git Bash, WSL, or similar):

```bash
make buildall
```

This will configure and build the project in one command.

## Manual Build Steps

If you don't have `make`, use these commands directly:

### 1. Configure the Project

```bash
cmake -G "Ninja Multi-Config" -B build
```

This generates the build files in the `build/` directory using Ninja as the build system.

### 2. Build the Project

```bash
cmake --build build
```

Or use Ninja directly:

```bash
ninja -C build
```

The compiled `which.exe` will be copied to the project root directory.

## Build Configurations

### Debug Build (Default)

```bash
cmake --build build --config Debug
```

Includes debug symbols for troubleshooting.

### Release Build

```bash
cmake -G "Ninja Multi-Config" -B build -DVERSION_INCLUDE_BUILDID=OFF --fresh
cmake --build build --config Release
```

Or using the Makefile:

```bash
make release
```

This creates an optimized release build without the build timestamp in the version string.

## CMake Options

You can customize the build with these CMake options:

### `USE_GOOGLE_TEST` (Default: OFF)

Enable Google Test framework and build unit tests:

```bash
cmake -G "Ninja Multi-Config" -B build -DUSE_GOOGLE_TEST=ON
```

### `VERSION_INCLUDE_BUILDID` (Default: ON)

Include a build timestamp in the version number. Set to OFF for release builds:

```bash
cmake -G "Ninja Multi-Config" -B build -DVERSION_INCLUDE_BUILDID=OFF
```

### `PROJECT_USES_GIT` (Default: ON)

Indicates the project uses Git for version control. This may be used for build statistics:

```bash
cmake -G "Ninja Multi-Config" -B build -DPROJECT_USES_GIT=OFF
```

## Testing

To build and run tests, first enable Google Test:

```bash
cmake -G "Ninja Multi-Config" -B build -DUSE_GOOGLE_TEST=ON
cmake --build build
```

Then run the tests:

```bash
ctest --test-dir build --output-on-failure
```

Or using the Makefile:

```bash
make test
```

## Static Analysis

Run static analysis tools to check code quality:

```bash
clang-tidy -p build src/*.cpp
cppcheck --enable=all --suppress=missingIncludeSystem -I src/include src/*.cpp
```

Or using the Makefile:

```bash
make check
```

**Note:** This requires `clang-tidy` and `cppcheck` to be installed and in your PATH.

## Cleaning Build Artifacts

To remove all build files and start fresh:

```bash
rm -rf build
rm -rf build-vscode
rm -f which.exe
```

Or using the Makefile:

```bash
make clean
```

## Troubleshooting

### Boost Not Found

If CMake can't find Boost, set the `BOOST_ROOT` environment variable:

```bash
set BOOST_ROOT=C:\path\to\boost
```

Or specify it when running CMake:

```bash
cmake -G "Ninja Multi-Config" -B build -DBOOST_ROOT=C:\path\to\boost
```

### Ninja Not Found

Make sure Ninja is installed and in your PATH. You can verify with:

```bash
ninja --version
```

### MSVC Not Found

If using MSVC, make sure you're running from a Visual Studio Developer Command Prompt, or have the Visual Studio environment variables set.

### Fresh Configuration

If you need to completely reconfigure the project (clear CMake cache):

```bash
cmake -G "Ninja Multi-Config" -B build --fresh
```

The `--fresh` flag removes the existing CMake cache before configuring.

## Creating a Release

The project uses GitHub Actions to automatically build and publish releases. When you push a version tag, the workflow will build the binary and create a GitHub Release with the executable attached.

### Release Process

1. **Ensure your code is ready for release:**
   - All changes are committed
   - Tests pass (if enabled)
   - Code builds successfully

2. **Create a version tag:**
   ```bash
   git tag v4.1.0
   ```

   Use semantic versioning: `vMAJOR.MINOR.PATCH` (e.g., `v4.1.0`, `v4.2.0`, `v5.0.0`)

3. **Push the tag to GitHub:**
   ```bash
   git push origin v4.1.0
   ```

   Or push all tags:
   ```bash
   git push --tags
   ```

4. **GitHub Actions automatically:**
   - Detects the new tag
   - Builds `which.exe` using MSVC in Release configuration
   - Creates a GitHub Release with auto-generated release notes
   - Uploads `which.exe` as a downloadable asset

5. **Monitor the build:**
   - Go to your repository on GitHub
   - Click the "Actions" tab
   - Watch the "Build and Release" workflow run
   - If successful, the release will appear on the [Releases page](https://github.com/johneegeek/which/releases)

### Release Notes

The GitHub Action automatically generates release notes from commit messages between tags. To make your release notes more informative:

- Write clear, descriptive commit messages
- Use conventional commit format if desired (e.g., `feat:`, `fix:`, `docs:`)
- The first line of commit messages will appear in the release notes

### Manual Edits

After the release is created, you can manually edit it on GitHub to:
- Add highlights or important changes
- Add upgrade instructions
- Add breaking changes warnings
- Customize the description

### Deleting or Re-releasing

If you need to recreate a release:

1. Delete the release on GitHub (go to Releases → click the release → Delete)
2. Delete the tag locally and remotely:
   ```bash
   git tag -d v4.1.0
   git push origin :refs/tags/v4.1.0
   ```
3. Create the tag again and push

**Note:** Avoid deleting releases that users may have already downloaded.

## Makefile Reference

For convenience, the project includes a Makefile with these targets:

- `make buildall` - Configure and build in one step
- `make build` - Incremental build (faster, uses existing configuration)
- `make configure` - Only run CMake configuration
- `make release` - Create optimized release build
- `make test` - Run unit tests
- `make check` - Run static analysis
- `make clean` - Remove all build artifacts
