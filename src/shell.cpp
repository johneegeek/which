/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <array>
#include <memory>
#include <string>
#include <stdexcept>

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