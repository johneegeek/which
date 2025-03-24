# WHICH Command for Windows

Spending a good portion of my time living in the command line, I found in Windows I kept typing `which <command>` only to receive the all too familar:

```shell
'which' is not recognized as an internal or external command, operable program or batch file.
```

One day, I sighed and said. "Fine, I'll make one"

It's modeled of the Linux and MacOS shell command, and is only sparsly documented as it was kind of quick and dirty.

Feel free to use if you like.

## Requirements to Build

- [Boost](https://www.boost.org/) C++ Libraries
- Some type of Windows Compiler
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/)
