/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdio>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "powershell.h"
#include "shell.h"

/**
 * @brief Search aliases (DOSKEY) for the given command.
 *
 * @param command Name of the command to search for.
 * @return std::vector<std::string> List of all matches in the aliases list
 */
std::vector<std::string> search_aliases(const std::string& command)
{
    std::vector<std::string> result;

    std::string command_output;
    try {
        if (is_powershell()) {
            std::string alias_command
                = "powershell.exe -Command \"Get-Alias | Where-Object Name -EQ '\""
                  + command + "'";
            command_output = exec(alias_command.c_str());
        }
        else {
            command_output = exec("doskey /macros");
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    if (command_output.empty()) { return result; }

    std::vector<std::string> tokens;
    boost::split(tokens, command_output, boost::is_any_of("\n"));

    const std::regex pattern{"(\\S+)=(.*)"};
    const std::regex pattern_ps{"Alias\\s*?(\\S+)\\s+->\\s+(.*?)$"};
    std::regex       search_pattern = pattern;
    if (is_powershell()) { search_pattern = pattern_ps; }

    std::smatch matches;
    for (const auto& token: tokens) {
        if (std::regex_match(token, matches, search_pattern)) {
            const std::string key   = matches[1];
            const std::string value = matches[2];
            if (boost::iequals(key, command)) {
                std::string message;
                message = "`" + command + "` is an alias for `"
                          + boost::trim_copy(value) + "`";
                result.push_back(message);
            }
        }
    }

    return result;
}