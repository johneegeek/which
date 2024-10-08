#include <array>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

// Helper function to execute a command and return the output.
std::string exec(const char* cmd)
{
    std::array<char, 128>                     buffer;
    std::string                               result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) { throw std::runtime_error("popen() failed!"); }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get())
           != nullptr) {
        result += buffer.data();
    }
    return result;
}

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
        command_output = exec("doskey /macros");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    if (command_output.empty()) { return result; }

    std::vector<std::string> tokens;
    boost::split(tokens, command_output, boost::is_any_of("\n"));

    const std::regex pattern{"(\\S+)=(.*)"};
    std::smatch matches;
    for (const auto& token: tokens) {
        if (std::regex_match(token, matches, pattern)) {
            const std::string key   = matches[1];
            const std::string value = matches[2];
            if (boost::iequals(key, command)) {
                std::string message;
                message = "`" + command + "` is an alias for `" + value + "`.";
                result.push_back(message);
            }
        }
    }

    return result;
}