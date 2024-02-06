#include "cxxopts.hpp"
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    cxxopts::Options options("which", "Prints the path of the executable");

    options.add_options()("a, all", "Show all matches")("s, silent",
                                                        "Don't display any output")(
        "h, help", "Prints this help message")("v, version", "Prints the version");

    auto result = options.parse(argc, argv);

    if(result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
}
