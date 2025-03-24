/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef POWERSHELL_H_
#define POWERSHELL_H_

#include <string>
#include <windows.h>
#include <tlhelp32.h> // This must be included after windows.h :(

extern bool is_powershell();

extern std::string powershell_cmd_match(const std::string& command);

#endif