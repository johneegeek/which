/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <boost/algorithm/string/predicate.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "shell.h"
#include "powershell.h"

/**
 * @brief Get the Parent Process Name object
 *
 * @return std::string
 */
std::string getParentProcessName()
{
    DWORD       currentPID        = GetCurrentProcessId();
    DWORD       parentPID         = 0;
    std::string parentProcessName = "";

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) { return parentProcessName; }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (processEntry.th32ProcessID == currentPID) {
                parentPID = processEntry.th32ParentProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    // Find parent process name
    if (parentPID != 0) {
        if (Process32First(snapshot, &processEntry)) {
            do {
                if (processEntry.th32ProcessID == parentPID) {
                    parentProcessName = processEntry.szExeFile;
                    break;
                }
            } while (Process32Next(snapshot, &processEntry));
        }
    }

    CloseHandle(snapshot);
    return parentProcessName;
}

/**
 * @brief Determine if the current process is running in PowerShell
 *
 * @return bool True if the current process is running in PowerShell, false
 * otherwise.
 */
bool is_powershell()
{
    std::string parent_process_name = getParentProcessName();
    return (parent_process_name == "powershell.exe"
            || parent_process_name == "pwsh.exe");
}

/**
 * Get the known powershell cmds
 *
 * Will use the powershell command `Get-Command` to retrieve a list of known
 * powershell commands, then a vector with ("Command", "Type") pairs.
 *
 * @return std::vector<std::pair<std::string, std::string>>
 */
std::vector<std::pair<std::string, std::string>> get_powershell_cmds()
{
    std::vector<std::pair<std::string, std::string>> pairs;

    std::string command_output
        = exec("powershell.exe -Command \"Get-Command | Where-Object {$_.CommandType "
               "-ne 'Alias'} | Format-Table Name, CommandType -HideTableHeaders\"");

    std::istringstream stream(command_output);
    std::string        command, type;

    // Read line by line
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);

        // Extract command and type
        if (lineStream >> command >> type) { pairs.emplace_back(command, type); }
    }
    return pairs;
}

/**
 * Match the given command with known powershell commands. Will return description
 * of the found command if found. otherwise, an empty string will be returned.
 *
 * @param command The command to search for.

 * @return std::string Description of the found command otherwise, an empty string.
 */
std::string powershell_cmd_match(const std::string& command)
{
    std::vector<std::pair<std::string, std::string>> powershell_cmds
        = get_powershell_cmds();
    for (const auto& pair: powershell_cmds) {
        if (boost::iequals(pair.first, command)) {
            return std::string("`") + pair.first + "` is a PowerShell " + pair.second
                   + ".";
        }
    }
    return "";
}
