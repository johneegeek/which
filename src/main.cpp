// Windows Search Sequence:
// https://learn.microsoft.com/en-us/previous-versions//cc723564(v=technet.10)#command-search-sequence
//
// Windows OOP?:
//    1. DOSKEY (aliases)
//    2. INTERNAL COMMANDS
//    3. PATH SEARCH (/w PATHEXT)
//
// TODO: Add -s option.
// TODO: Env options
// TODO: Document
#include "aliases.h"
#include "internal_cmds.h"
#include "progargs.h"
#include "which.h"

#include <boost/range/join.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    int                     return_code = 0;
    std::string             command;

    auto prog_opts = parse_args(argc, argv);

    bool quiet = prog_opts.count("quiet");
    bool all   = prog_opts.count("all");
    bool show  = prog_opts.count("show");

    if (prog_opts.count("cmd")) { command = prog_opts["cmd"].as<std::string>(); }
    else {
        show_usage();
        exit(2);
    }

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
