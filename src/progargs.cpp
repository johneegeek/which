#include "cxxopts.hpp"
#include "which_version.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

enum { kMaxArgs = 32 }; // Maximum number of arguments. Way more than we need.

void show_version()
{
    std::cout << "which (" << WHICH_VERSION << ") for Windows. ";
    std::cout << "John Kiernan, 2024" << std::endl;
    std::cout << "Built on " << BUILD_DATE << "  (git sha: " << WHICH_GIT_SHA << ") ";
    std::cout << "[" << COMPILER_INFO << "]" << std::endl;
}

void show_usage()
{
    std::cout << "Usage: which [options] cmd" << std::endl;
    std::cout << "    use --help for more information" << std::endl;
}

//
// Helper function to append the command line arguments with any values
// that might be in the environment variable %WHICH%
//
//  Pass in a the original argc and argv, and a pointer to the new argv.
//  will return the number of arguments in the new argv.
//
int append_argv(const int argc, char* const* argv, char* new_argv[])
{
    int new_argc = 0;

    // Add the original command line arguments
    for (int i = 0; i < argc; i++) { new_argv[new_argc++] = argv[i]; }

    char* env_opts = std::getenv("WHICH");
    if (env_opts == nullptr) { return argc; }

    char* p = std::strtok(env_opts, " ");
    while (p && new_argc < kMaxArgs - 1) {
        new_argv[new_argc++] = p;

        p = std::strtok(nullptr, " ");
    }
    new_argv[new_argc] = nullptr;

    return new_argc;
}

// Parse the command line arguments using CXXOPTS
// https://github.com/jarro2783/cxxopts
//
cxxopts::ParseResult parse_args(int argc, char* argv[])
{
    char* nargv[kMaxArgs];

    // Append the command line arguments with any values that might be in the
    // environment variable %WHICH%
    const int nargc = append_argv(argc, argv, nargv);

    std::unique_ptr<cxxopts::Options> allocated_options(
        new cxxopts::Options("which", "Which for Windows\nReturns the pathnames of the "
                                      "file(s) (or links) which would be "
                                      "executed in the current environment.\n"));

    auto& options = *allocated_options;

    cxxopts::ParseResult result;

    try {
        // clang-format off
        options
            .positional_help("cmd");

        options
            .set_width(80)
            .add_options()
            ("h, help", "Show this help message and exit.")
            ("a, all", "List all matches, not just the first.")
            ("i, info", "Show file infomation: date/time and size.")
            ("s, silent", "Quietly check, exit code is 0 if found, otherwise non-zero.")
            ("v, version","Show version information.")
            ("cmd","Command to locate.", cxxopts::value<std::string>())
            ;
        // clang-format on

        options.parse_positional({"cmd"});

        result = options.parse(nargc, nargv);
    }

    catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        std::cerr << "Try --help for more information." << std::endl;
        exit(2);
    }

    if (static_cast<bool>(result.count("help"))) {
        std::cout << options.help() << std::endl;
        show_version();
        exit(0);
    }

    if (static_cast<bool>(result.count("version"))) {
        show_version();
        exit(0);
    }

    return result;
}