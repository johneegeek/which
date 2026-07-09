/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include "cxxopts.hpp"
#include "powershell.h"
#include "progargs.h"
#include "which_version.h"

#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

constexpr int kMaxArgs = 32; // Maximum number of arguments. Way more than we need.
constexpr int kHelpWidth = 80;

namespace {

void show_version()
{
    std::cout << "which (" << WHICH_VERSION << ") for Windows. ";
    std::cout << "John Kiernan, 2018-2026" << std::endl;
    if (is_powershell()) { std::cout << "Running under PowerShell" << std::endl; }
    std::cout << "Built on " << BUILD_DATE << "  (git sha: " << WHICH_GIT_SHA << ") ";
    std::cout << "[" << COMPILER_INFO << "]" << std::endl;
}

} // namespace

void show_usage()
{
    std::cout << "Usage: which [options] cmd" << std::endl;
    std::cout << "    use --help for more information" << std::endl;
}

namespace {

//
// Helper function to append the command line arguments with any values
// that might be in the environment variable %WHICH%
//
//  Pass in a the original argc and argv, and a reference to the new argv
//  buffer. Will return the number of arguments written into it.
//
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
// argv is main()'s raw char*[] as mandated by the C++ standard's required
// main() signature; there is no bounds-safe alternative for indexing it.
int append_argv(const int argc, char* const* argv, std::array<char*, kMaxArgs>& new_argv)
{
    int new_argc = 0;

    // Add the original command line arguments
    for (int i = 0; i < argc && new_argc < kMaxArgs; i++) {
        new_argv.at(static_cast<std::size_t>(new_argc)) = argv[i];
        ++new_argc;
    }

    char* env_opts = std::getenv("WHICH");
    if (env_opts == nullptr) { return new_argc; }

    char* token = std::strtok(env_opts, " ");
    while (token != nullptr && new_argc < kMaxArgs - 1) {
        new_argv.at(static_cast<std::size_t>(new_argc)) = token;
        ++new_argc;

        token = std::strtok(nullptr, " ");
    }
    new_argv.at(static_cast<std::size_t>(new_argc)) = nullptr;

    return new_argc;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

} // namespace

// Parse the command line arguments using CXXOPTS
// https://github.com/jarro2783/cxxopts
//
// NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
// argv here mirrors main()'s required signature, forwarded unchanged from
// main.cpp; it cannot be changed to a container type at this boundary.
cxxopts::ParseResult parse_args(int argc, char* argv[])
// NOLINTEND(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
{
    std::array<char*, kMaxArgs> nargv{};

    // Append the command line arguments with any values that might be in the
    // environment variable %WHICH%
    const int nargc = append_argv(argc, argv, nargv);

    const std::unique_ptr<cxxopts::Options> allocated_options(
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
            .set_width(kHelpWidth)
            .add_options()
            ("h, help", "Show this help message and exit.")
            ("a, all", "List all matches, not just the first.")
            ("i, info", "Show file infomation: date/time and size.")
            ("k, skip-aliases", "Skip checking aliases.")
            ("s, silent", "Quietly check, exit code is 0 if found, otherwise non-zero.")
            ("v, version","Show version information.")
            ("cmd","Command to locate.", cxxopts::value<std::string>())
            ;
        // clang-format on

        options.parse_positional({"cmd"});

        result = options.parse(nargc, nargv.data());
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