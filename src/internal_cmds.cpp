#include "internal_cmds.h"

#include <boost/algorithm/string.hpp>
#include <set>
#include <string>
#include <vector>

const std::set<std::string> INTERNAL_COMMANDS
    = {"ASSOC",  "BREAK", "CALL",  "CD",     "CHDIR",    "CLS",      "COLOR", "COPY",
       "DATE",   "DEL",   "DIR",   "DPATH",  "ECHO",     "ENDLOCAL", "ERASE", "EXIT",
       "FOR",    "FTYPE", "GOTO",  "IF",     "KEYS",     "MD",       "MKDIR", "MKLINK",
       "MOVE",   "PATH",  "PAUSE", "POPD",   "PROMPT",   "PUSHD",    "REM",   "REN",
       "RENAME", "RD",    "RMDIR", "SET",    "SETLOCAL", "SHIFT",    "START", "TIME",
       "TITLE",  "TYPE",  "VER",   "VERIFY", "VOL"};

bool is_internal_command(const std::string& command)
{
    std::string command_str(command);
    boost::algorithm::to_upper(command_str);
    return INTERNAL_COMMANDS.find(command_str.c_str()) != INTERNAL_COMMANDS.end();
}

/**
 * @brief Search for built-in commands.
 *
 * There is not really good way to determine if a command is a built-in command,
 * so the best we can do is to check if the command is in the list of known built-ins.
 *
 * @param command Command to search for.
 * @return std::vector<std::string> Returning it as a vector of strings to make merging
 * easier.
 */
std::vector<std::string> search_internal_commands(const std::string& command)
{
    std::vector<std::string> result;

    if (is_internal_command(command)) {
        const std::string cmdstr = boost::algorithm::to_upper_copy(command);
        std::string message = cmdstr + " is an internal Windows command. (CMD.EXE)";
        result.push_back(message);
    }

    return result;
}