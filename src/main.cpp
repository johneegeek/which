// Windows Search Sequence:
// https://learn.microsoft.com/en-us/previous-versions//cc723564(v=technet.10)#command-search-sequence
//
// Windows OOP?:
//    1. DOSKEY (aliases)
//    2. INTERNAL COMMANDS
//    3. PATH SEARCH (/w PATHEXT)
//
#include "cxxopts.hpp"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    cxxopts::Options options("which", "Prints the path of the executable");

    // clang-format off
    options.add_options()
    ("a, all", "Show all matches")
    ("s, silent", "Don't display any output")
    ("h, help", "Prints this help message")
    ("v, version", "Prints the version")
    ;
    // clang-format on

    auto result = options.parse(argc, argv);

    if(result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
}
