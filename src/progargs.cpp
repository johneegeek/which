#include "cxxopts.hpp"
#include "which_version.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

enum { kMaxArgs = 32 }; // Maximum number of arguments. Way more than we need.

void show_version()
{
    std::cout << "Version " << WHICH_VERSION << std::endl;
    std::cout << "John Kiernan, 2024  - Git SHA(" << WHICH_GIT_SHA << ")" << std::endl;
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
    if (env_opts == nullptr) return argc;

    char* p = std::strtok(env_opts, " ");
    while (p && new_argc < kMaxArgs - 1) {
        new_argv[new_argc++] = p;

        p = std::strtok(nullptr, " ");
    }
    new_argv[new_argc] = nullptr;

    return new_argc;
}

cxxopts::ParseResult parse_args(int argc, char* argv[])
{
    std::unique_ptr<cxxopts::Options> allocated_options(
        new cxxopts::Options("which", "Which for Windows\nReturns the pathnames of the "
                                      "file(s) (or links) which would be "
                                      "executed in the current environment.\n"));

    auto& options = *allocated_options;

    try {
        // clang-format off
        options
            .positional_help("cmd");

        options
            .set_width(80)
            .add_options()
            ("a, all", "List all matches, not just the first.")
            ("s, show", "Show file date/time and size.")
            ("q, quiet", "Quietly check, exit code is 0 if found, otherwise non-zero.")
            ("h, help", "Show this help message and exit.")
            ("v, version","Show version information.")
            ("cmd","Command to locate.", cxxopts::value<std::string>())
            ;
        // clang-format on

        options.parse_positional({"cmd"});
    }
    catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        std::cerr << "Try --help for more information." << std::endl;
        exit(2);
    }

    char* nargv[kMaxArgs];

    int nargc = append_argv(argc, argv, nargv);

    // Parse the command line arguments and environment arguments.
    auto result = options.parse(nargc, nargv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        show_version();
        exit(0);
    }

    if (result.count("version")) {
        show_version();
        exit(0);
    }

    return result;
}