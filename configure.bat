:: Configuration helper for IGS build system
::
:: By default it will try to just use Visual Studio
:: However, you can also use `Ninja` if you have it installed
:: on your system. Visual Studio acutally uses `ninja` inside Cmake build
:: projects.
:: 
:: To use with `ninja`:
::      (1) Set the VS environment: 
::          C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat x64
::
::      (2) set CMAKE_GENERATOR=Ninja
::              If this is set, we'll try to do #1 for you.
::
@echo off

if NOT "%CMAKE_GENERATOR%" == "" (
    set CMAKE_GENERATOR_PLATFORM=
    if "%VCINSTALLDIR%" == ""  (
        if not exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
            echo ERROR: There is no Visual Studio installation found for building with Ninja
            echo        [You need to call `vcvarsall.bat`]
            goto end
        )
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
    )
)

:: Default is to use Visual Studio
if  "%CMAKE_GENERATOR%_" == "_" (
    set CMAKE_GENERATOR=Visual Studio 17 2022
    set CMAKE_GENERATOR_PLATFORM=x64
)
echo CMake Generator: %CMAKE_GENERATOR%

if defined CMAKE_GENERATOR_PLATFORM (
    echo Platform: %CMAKE_GENERATOR_PLATFORM%
)
echo ------------------------------------------------

cmake %* -B build

:end