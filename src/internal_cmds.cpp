/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <boost/algorithm/string/case_conv.hpp>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "internal_cmds.h"
#include "match_result.h"
#include "powershell.h"

namespace {

const std::set<std::string>& internal_commands()
{
    static const std::set<std::string> commands
        = {"ASSOC",  "BREAK", "CALL",  "CD",     "CHDIR",    "CLS",      "COLOR", "COPY",
           "DATE",   "DEL",   "DIR",   "DPATH",  "ECHO",     "ENDLOCAL", "ERASE", "EXIT",
           "FOR",    "FTYPE", "GOTO",  "IF",     "KEYS",     "MD",       "MKDIR", "MKLINK",
           "MOVE",   "PATH",  "PAUSE", "POPD",   "PROMPT",   "PUSHD",    "REM",   "REN",
           "RENAME", "RD",    "RMDIR", "SET",    "SETLOCAL", "SHIFT",    "START", "TIME",
           "TITLE",  "TYPE",  "VER",   "VERIFY", "VOL"};
    return commands;
}

} // namespace

bool is_internal_command(const std::string& command)
{
    std::string command_str(command);
    boost::algorithm::to_upper(command_str);
    return internal_commands().find(command_str) != internal_commands().end();
}

/**
 * @brief Search for built-in commands.
 *
 * There is not really good way to determine if a command is a built-in command,
 * so the best we can do is to check if the command is in the list of known built-ins.
 *
 * @param command Command to search for.
 * @return std::vector<MatchResult> Returning it as a vector to make merging easier.
 * An internal command is never a real file, so `path` is always unset on the
 * returned entries.
 */
std::vector<MatchResult> search_internal_commands(const std::string& command)
{
    std::vector<MatchResult> result;

    if (is_powershell()) {
        const std::string matched = powershell_cmd_match(command);
        if (!matched.empty()) { result.push_back(MatchResult{matched, std::nullopt}); }
        return result;
    }

    if (is_internal_command(command)) {
        const std::string cmdstr = boost::algorithm::to_upper_copy(command);
        const std::string message = cmdstr + " is an internal Windows command. (CMD.EXE)";
        result.push_back(MatchResult{message, std::nullopt});
    }

    return result;
}
