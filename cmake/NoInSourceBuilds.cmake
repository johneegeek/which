# Don't allow in-source builds to keep the mess out of the source.
# If you attempt an in-source build, you will get a message but Cmake
# still leave some junk in the directory you attempted to build from.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
        "\n"
        "In-source builds are not allowed to keep the mess out of the source. "
        "Instead, use a path to a build directory, for example:\n"
        "  cmake -B build"
        "\n"
        "To remove the files you just accidentally created execute:\n"
        "  rm -rf CMakeFiles CMakeCache.txt\n"
    )
endif()