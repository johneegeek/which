# WHICH Command for Windows

Spending a good portion of my time living in the command line, I found in Windows I kept typing `which <command>` only to receive the all too familar:

```shell
'which' is not recognized as an internal or external command, operable program or batch file.
```

One day, I sighed and said. "Fine, I'll make one"

It's modeled of the Linux and MacOS shell command, and is only sparsly documented as it was kind of quick and dirty.

Feel free to use if you like.

## Table of Contents

- [What is `which`?](#what-is-which)
- [Usage](#usage)
  - [Basic Usage](#basic-usage)
  - [Command-Line Options](#command-line-options)
  - [Examples](#examples)
  - [Environment Variable](#environment-variable)
- [Binaries](#binaries)
  - [Installation](#installation)
- [Building from Source](#building-from-source)
  - [Quick Requirements](#quick-requirements)

## What is `which`?

`which` locates executables in your Windows environment by searching through the same sequence that Windows uses when you type a command:

1. **Aliases** - DOSKEY macros in CMD.EXE, or PowerShell aliases
2. **Internal Commands** - Built-in CMD.EXE commands (like `cd`, `dir`, `copy`) or PowerShell cmdlets
3. **PATH Search** - Executables in directories listed in your `PATH` environment variable, using extensions from `PATHEXT`

The tool automatically detects whether you're running in CMD.EXE or PowerShell and adjusts its search behavior accordingly.

## Usage

### Basic Usage

Find the location of a command:

```cmd
which notepad
```

Output:
```
C:\Windows\System32\notepad.exe
```

### Command-Line Options

**Show all matches** (not just the first):
```cmd
which --all python
```

**Show file information** (size and date):
```cmd
which --info git
```

Output:
```
12/15/2024 03:45:30 PM 2.5MB (2621440)  C:\Program Files\Git\cmd\git.exe
```

**Silent mode** (exit code only):
```cmd
which --silent node
echo %ERRORLEVEL%
```

Returns `0` if found, `1` if not found. Useful in scripts.

**Version information:**
```cmd
which --version
```

**Help:**
```cmd
which --help
```

### Examples

**Check if a command exists in a script:**
```batch
which --silent docker
if %ERRORLEVEL% == 0 (
    echo Docker is installed
) else (
    echo Docker not found
)
```

**Find all Python installations:**
```cmd
which --all python
```

**Check what an alias points to:**
```cmd
which ls
```
Output: `` `ls` is an alias for `Get-ChildItem` `` (in PowerShell)

**Get details about a command:**
```cmd
which --info --all cmake
```

### Environment Variable

You can set default options using the `WHICH` environment variable:

```cmd
set WHICH=--all
which python
```

Now `which` will always show all matches by default.

## Binaries

Pre-built binaries are available on the [Releases page](https://github.com/johneegeek/which/releases).

### Installation

1. Download `which.exe` from the latest release
2. Create a folder for your utilities (e.g., `C:\bin` or `C:\Users\YourName\bin`)
3. Copy `which.exe` to that folder
4. Add the folder to your PATH environment variable:
   - Press `Win + X` and select "System"
   - Click "Advanced system settings"
   - Click "Environment Variables"
   - Under "User variables" or "System variables", find and select "Path"
   - Click "Edit" → "New"
   - Add the path to your folder (e.g., `C:\bin`)
   - Click "OK" to save
5. Open a new command prompt or PowerShell window and type `which` to verify

## Building from Source

For detailed build instructions, see [BUILD.md](BUILD.md).

### Quick Requirements

- [Boost](https://www.boost.org/) C++ Libraries
- Some type of Windows Compiler (MSVC, MinGW, or Clang)
- [CMake](https://cmake.org/) (3.24+)
- [Ninja](https://ninja-build.org/)
