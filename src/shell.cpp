/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <array>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

#include "shell.h"

constexpr std::size_t kBufferSize = 128;

// Helper function to execute a command and return the output.
// NOLINTBEGIN(misc-include-cleaner)
// _popen/_pclose are MSVC CRT extensions declared in <cstdio>, but like the
// Windows SDK, have no entry in misc-include-cleaner's symbol database.
std::string exec(const char* cmd)
{
    std::array<char, kBufferSize>                   buffer{};
    std::string                                     result;
    // exec() intentionally shells out to run arbitrary commands (doskey,
    // powershell, etc.) - that is its entire purpose. Callers are responsible
    // for not passing unsanitized input; see search_aliases() in aliases.cpp,
    // which currently concatenates user input into the command string and is
    // the actual place any injection risk needs to be addressed.
    // NOLINTNEXTLINE(bugprone-command-processor)
    const std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) { throw std::runtime_error("popen() failed!"); }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get())
           != nullptr) {
        result += buffer.data();
    }
    return result;
}
// NOLINTEND(misc-include-cleaner)