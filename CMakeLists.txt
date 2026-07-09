cmake_minimum_required(VERSION 3.24)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
) 
# Don't build from inside the source tree
include( NoInSourceBuilds )

project(which
    VERSION 4.2.1
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)

set( PROJECT_ROOT ${CMAKE_SOURCE_DIR} )
set( PROJECT_SRC_DIR ${PROJECT_ROOT}/src )
set( PROJECT_BIN_DIR ${PROJECT_ROOT} )

option( USE_GOOGLE_TEST
    "Use Google Test" OFF
)

option( PROJECT_USES_GIT
    "Project is using git" ON
)

option( VERSION_INCLUDE_BUILDID
    "Inlude build id (timestamp) as part of the version number" ON
)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Just incase the VC++ runtime is not installed, we'll 
# statically link it. It will make the exe a bit bigger, but
# guaranteed to work.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(_CRT_SECURE_NO_DEPRECATION _CRT_SECURE_NO_WARNINGS)
if (MSVC)
    add_compile_options("$<$<COMPILE_LANGUAGE:C,CXX>:/W4>")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:C,CXX>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wconversion;-Wsign-conversion;-Wnon-virtual-dtor;-Woverloaded-virtual;-Wnull-dereference;-Wdouble-promotion;-Wformat=2;-Wimplicit-fallthrough>"
    )
endif()

# Configure the version information
include(BuildStats)
configure_file(
    ${PROJECT_SRC_DIR}/include/which_version.h.in
    ${CMAKE_BINARY_DIR}/which_version.h
    @ONLY
)


# Get me the boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED )

set(main-src 
    src/main.cpp
)
set(lib-src
    src/aliases.cpp
    src/internal_cmds.cpp
    src/progargs.cpp
    src/shell.cpp
    src/which.cpp
    src/powershell.cpp
)

include_directories(
        ${PROJECT_SRC_DIR}
        ${PROJECT_SRC_DIR}/include
        ${CMAKE_BINARY_DIR}
)


add_library( whichlib STATIC ${lib-src} )
target_link_libraries(whichlib PUBLIC Boost::headers)
add_executable(which ${main-src})

target_link_libraries(which PUBLIC
    whichlib
    Boost::headers
)

add_custom_command( TARGET which
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                    -E copy $<TARGET_FILE:which> ${PROJECT_BIN_DIR} 
                    COMMENT "Performing POST_BUILD commands"
)


if (USE_GOOGLE_TEST) 
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    message(STATUS "Setting up Google Test")
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
    enable_testing()
endif()

