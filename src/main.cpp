// Windows Search Sequence:
// https://learn.microsoft.com/en-us/previous-versions//cc723564(v=technet.10)#command-search-sequence
//
// Windows OOP?:
//    1. DOSKEY (aliases)
//    2. INTERNAL COMMANDS
//    3. PATH SEARCH (/w PATHEXT)
//
// TODO: Add -s option.
// TODO: Add version information
// TODO: Document
#include "aliases.h"
#include "internal_cmds.h"
#include "which.h"

#include <boost/program_options.hpp>
#include <boost/range/join.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace po = boost::program_options;

void show_version()
{
    std::cout << "TODO: version info." << std::endl;
}

int main(int argc, char* argv[])
{
    int                     return_code = 0;
    std::string             command;
    po::options_description options("Options");

    // clang-format off
    options.add_options()
    ("all,a", "List all matches, not just the first one.")
    ("show,s", "Show file date/time and size.")
    ("quiet,q", "Quietly check, exit code 0 if found, otherwise non-zero.")
    ("help,h", "Prints this help message and exit.")
    ("version,v", "Prints the version.")
    ;
    // clang-format on
    //

    po::options_description hidden;
    hidden.add_options()("command",
                         boost::program_options::value<std::string>(&command),
                         "The command to search for.");

    po::positional_options_description positional_options;
    positional_options.add("command", -1);

    po::options_description all_options;
    all_options.add(options);
    all_options.add(hidden);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                      .options(all_options)
                      .positional(positional_options)
                      .run(),
                  vm);

        po::notify(vm);
    }
    catch (boost::program_options::error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << options << std::endl;
        return 2;
    }

    if (vm.count("help")) {
        std::cout << "Usage: which [options] command\n\n";
        std::cout << options << std::endl;
        show_version();
        return 0;
    }

    if (vm.count("version")) {
        show_version();
        return 0;
    }

    bool quiet = vm.count("quiet");
    bool all   = vm.count("all");
    bool show  = vm.count("show");

    std::vector<std::string> alias_search  = search_aliases(command);
    std::vector<std::string> internal_cmds = search_internal_commands(command);
    std::vector<std::string> main_search   = search_path(command);
    std::vector<std::string> results;

    auto range1     = boost::join(alias_search, internal_cmds);
    auto full_range = boost::join(range1, main_search);
    for (auto it = boost::begin(full_range); it != boost::end(full_range); ++it) {
        results.push_back(*it);
    }

    if (results.empty())
        return_code = 1;
    else if (!quiet)
        std::cout << results[0] << std::endl;

    if (quiet) return return_code;

    if ((results.size() > 1) && all) {
        // std::cout << "\n(Also found)\n------------" << std::endl;
        for (int i = 1; i < results.size(); ++i) {
            std::cout << results[i] << std::endl;
        }
        return return_code;
    }

    return return_code;
}
