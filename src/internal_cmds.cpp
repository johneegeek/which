#include "internal_cmds.h"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <set>
#include <string>

const std::set<std::string> INTERNAL_COMMANDS
    = {"ASSOC",  "BREAK", "CALL",  "CD",     "CHDIR",    "CLS",      "COLOR", "COPY",
       "DATE",   "DEL",   "DIR",   "DPATH",  "ECHO",     "ENDLOCAL", "ERASE", "EXIT",
       "FOR",    "FTYPE", "GOTO",  "IF",     "KEYS",     "MD",       "MKDIR", "MKLINK",
       "MOVE",   "PATH",  "PAUSE", "POPD",   "PROMPT",   "PUSHD",    "REM",   "REN",
       "RENAME", "RD",    "RMDIR", "SET",    "SETLOCAL", "SHIFT",    "START", "TIME",
       "TITLE",  "TYPE",  "VER",   "VERIFY", "VOL"};

bool is_internal_command(const char* command)
{
    std::string s(command);
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return INTERNAL_COMMANDS.find(s.c_str()) != INTERNAL_COMMANDS.end();
}
