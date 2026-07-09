// Windows version of the WHICH command.
// John Kiernan, 2018-2025
//
// Searches aliases (DOSKEY), internal commands (CMD.EXE) and the search path.
// On Powershell, it will search known CMDLets as well.
//
// Windows Search Sequence:
// https://learn.microsoft.com/en-us/previous-versions//cc723564(v=technet.10)#command-search-sequence
//
// Windows OOP:
//    1. DOSKEY (aliases)
//    2. INTERNAL COMMANDS
//    3. PATH SEARCH (/w PATHEXT)
//
// License: MIT License (https://opensource.org/licenses/MIT)
//
#include "aliases.h"
#include "internal_cmds.h"
#include "progargs.h"
#include "which.h"

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/join.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

// The exceptions main() can theoretically propagate here are not reachable in
// practice: cxxopts::ParseResult::operator[] only throws for a missing key,
// which is already guarded above by prog_opts.count("cmd"); iostream failures
// require an explicit exceptions() mask this codebase never sets. Everything
// else that can realistically throw is handled by the try/catch below.
// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[])
{
    int                     return_code = 0;
    std::string             command;

    auto prog_opts = parse_args(argc, argv);

    const bool silent = static_cast<bool>(prog_opts.count("silent"));
    const bool all    = static_cast<bool>(prog_opts.count("all"));
    const bool info   = static_cast<bool>(prog_opts.count("info"));
    const bool skip_alias = static_cast<bool>(prog_opts.count("skip-aliases"));

    if (static_cast<bool>(prog_opts.count("cmd"))) {
        // cxxopts::ParseResult::operator[] performs its own key lookup and throws
        // if the key is missing; there is no unchecked raw-container access here.
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
        command = prog_opts["cmd"].as<std::string>();
    }
    else {
        show_usage();
        exit(2);
    }

    std::vector<std::string> results;

    try {
        std::vector<std::string> alias_search;
        if (!skip_alias) {
            alias_search = search_aliases(command);
        }
        std::vector<std::string> internal_cmds = search_internal_commands(command);
        std::vector<std::string> main_search   = search_path(command, info);

        // Combine all the search results int one vector.
        auto range1     = boost::join(alias_search, internal_cmds);
        auto full_range = boost::join(range1, main_search);
        for (auto it = boost::begin(full_range); it != boost::end(full_range); ++it) {
            results.push_back(*it);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return_code = 1;
    }

    // Print the first match (unless --silent   )
    if (results.empty()) { return_code = 1; }
    else if (!silent) {
        std::cout << results.at(0) << std::endl;
    }

    if (silent) { return return_code; }

    // If there are more than one matches, print them all (if --all)
    if ((results.size() > 1) && all) {
        // std::cout << "\n(Also found)\n------------" << std::endl;
        for (size_t i = 1; i < results.size(); ++i) {
            std::cout << results.at(i) << std::endl;
        }
        return return_code;
    }

    return return_code;
}
