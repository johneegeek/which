/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "match_result.h"
#include "powershell.h"
#include "which.h"

namespace {

    /**
     * @brief Convert a time point to a time_t value
     *
     * This function converts a time point to a time_t value by casting the
     * time point to a system_clock::duration and adding the current system time
     * to it. The result is then cast back to a time_t value.
     *
     * Use this to convert a std::filesystem::file_time_type to a time_t value.
     *
     * @tparam TP The type of time point to convert
     * @param tp The time point to convert
     * @return time_t The converted time_t value
     */
    template<typename TP> std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                            + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    /**
     * @brief A struct to pretty print a date time
     *
     * This struct is used to pretty print a date time in a human readable format.
     */
    struct PrettyDateTime {
            std::filesystem::file_time_type ftime;

        private:
            /**
             * @brief output stream operator
             *
             * This function is used to output the date time in a human readable format.
             *
             * @param os the output stream
             * @param pdt the pretty date time to output
             * @return std::ostream& the output stream
             */
            friend std::ostream& operator<<(std::ostream& os, PrettyDateTime pdt)
            {
                std::time_t ttime  = to_time_t(pdt.ftime);
                std::tm*    localt = std::localtime(&ttime);
                os << std::put_time(localt, "%m/%d/%Y %I:%M:%S %p");
                return os;
            }
    };

    /**
     * @brief A struct to pretty print a file size
     *
     * This struct is used to pretty print a file size in a human readable format.
     */
    struct PrettySize {
            std::uintmax_t size{};

        private:
            /**
             * @brief output stream operator
             *
             * This function is used to output the file size in a human readable format.
             *
             * @param os the output stream
             * @param pdt the pretty file size to output
             * @return std::ostream& the output stream
             */
            friend std::ostream& operator<<(std::ostream& os, PrettySize pfs)
            {
                static constexpr double              kUnitBase       = 1024.0;
                static constexpr double              kRoundingFactor = 10.0;
                static constexpr std::array<char, 7> kUnitSuffixes{'B', 'K', 'M', 'G',
                                                                   'T', 'P', 'E'};

                auto mantissa   = static_cast<double>(pfs.size);
                int  unit_index = 0;
                while (mantissa >= kUnitBase
                       && unit_index < static_cast<int>(kUnitSuffixes.size() - 1)) {
                    mantissa /= kUnitBase;
                    ++unit_index;
                }
                os << std::ceil(mantissa * kRoundingFactor) / kRoundingFactor
                   << kUnitSuffixes.at(static_cast<std::size_t>(unit_index));
                return (unit_index != 0) ? os << "B (" << pfs.size << ')' : os;
            }
    };

    /**
     * Get the set of file extensions (lowercase, with leading dot) that --edit
     * considers safe/sensible to hand to %EDITOR%. This is deliberately just
     * text/script/source files, not executables, archives, etc.
     *
     * @return const std::set<std::string>&
     */
    const std::set<std::string>& editable_extensions()
    {
        static const std::set<std::string> extensions
            = {".py",  ".pyw",  ".bat",  ".cmd",  ".js",   ".vbs",  ".vbe",  ".jse",
               ".ps1", ".psm1", ".psd1", ".txt",  ".ini",  ".cfg",  ".conf", ".json",
               ".xml", ".yaml", ".yml",  ".md",   ".html", ".htm",  ".css",  ".c",
               ".h",   ".cpp",  ".hpp",  ".cs",   ".java", ".sh",   ".ts",   ".sql",
               ".toml", ".log"};
        return extensions;
    }

} // namespace

/**
 * @brief Get the directories in the PATH environment variable
 *
 * This function returns a vector of directories in the PATH environment variable.
 * The current directory is always included in the list.
 *
 * @return std::vector<std::filesystem::path> A vector of directories in the PATH
 */
std::vector<std::filesystem::path> get_path_dirs()
{
    std::vector<std::filesystem::path> path_dirs;

    // Make sure we check the current directory
    // In powershell the local directory is NOT searched. Don't know why.
    if (!is_powershell()) {
        path_dirs.emplace_back("./");
    }

    // Get the PATH from the environment
    const char* path = std::getenv("PATH");
    std::string path_s(path);

    std::vector<std::string> tokens;
    boost::split(tokens, path_s, boost::is_any_of(";"));

    for (const auto& token: tokens) {
        // Check if the token is a directory reduce checking bad paths.
        if (std::filesystem::is_directory(token)) {
            path_dirs.emplace_back(token);
        }
    }

    return path_dirs;
}

/**
 * @brief Get a list of all the extensions in PATHEXT.
 *
 * @return std::vector<std::string> A vector of all the extensions in PATHEXT.
 */
std::vector<std::string> get_path_exts()
{
    std::vector<std::string> path_exts;

    // Get the PATH from the environment
    const char* extensions = std::getenv("PATHEXT");
    std::string path_exts_s(extensions);

    std::vector<std::string> tokens;
    boost::split(tokens, path_exts_s, boost::is_any_of(";"));

    for (auto& token: tokens) {
        boost::algorithm::to_lower(token); // Make it lowercase to look nicer.
        path_exts.push_back(token);
    }

    return path_exts;
}

/**
 * @brief A function to get all possible files with a given name and extension
 *
 * This function takes a filename and returns all possible files with the same name
 * and different extensions from the PATEXT environment variable.
 *
 * @param filename The filename to search for
 * @return std::vector<std::filesystem::path> A vector of files with the given name
 * and different extensions
 */
std::vector<std::filesystem::path> files_to_check(const std::string& filename)
{
    std::vector<std::filesystem::path> files;
    std::filesystem::path              path(filename);

    const std::vector<std::string> extensions = get_path_exts();
    for (const auto& ext: extensions) { files.push_back(path.replace_extension(ext)); }

    return files;
}

/**
 * @brief Check if a file is one we're willing to hand to %EDITOR% for --edit.
 *
 * @param file Path to check.
 * @return bool True if the file's extension is considered editable.
 */
bool is_editable_file(const std::filesystem::path& file)
{
    std::string ext = file.extension().string();
    boost::algorithm::to_lower(ext);
    return editable_extensions().find(ext) != editable_extensions().end();
}

/**
 * @brief Search for a file in the PATH environment variable
 *
 * @param filename Name of the file to search for
 * @param show_info Show the size and date of the file
 * @return std::vector<MatchResult> A vector of matches, each carrying the real
 * filesystem path when one is known.
 */
std::vector<MatchResult> search_path(const std::string& filename, bool show_info)
{
    std::vector<MatchResult> found_matches;

    const std::vector<std::filesystem::path> check_list = files_to_check(filename);
    const std::vector<std::filesystem::path> path_dirs  = get_path_dirs();
    for (const auto& _path: path_dirs) {
        for (const auto& check: check_list) {
            std::filesystem::path findme = _path / check;
            std::error_code       ec; // Using noexcept versions but ignoring the ec.
            const std::filesystem::file_status fstatus
              = std::filesystem::status(findme, ec);
            std::stringstream            ss;
            if (std::filesystem::is_regular_file(fstatus)
                || std::filesystem::is_symlink(fstatus)) {
                if (show_info) {
                    ss << PrettyDateTime{std::filesystem::last_write_time(findme, ec)}
                       << " " << PrettySize{std::filesystem::file_size(findme, ec)}
                       << "\t ";
                }
                ss << findme.make_preferred().string();
                found_matches.push_back(MatchResult{ss.str(), findme});
            }
            else if (fstatus.type() == std::filesystem::file_type::none) {
                // This is the dubious case where it's a weird Microsoft link to an
                // executable in the WinApps dir. We can't read the time or size
                // properly so we will show it a bit different.
                if (show_info) { ss << "(Windows App Execution Alias)\t\t "; }
                else {
                    ss << "@";
                }
                ss << findme.make_preferred().string();
                // We can't reliably treat these App Execution Alias entries as a
                // normal file, so leave the path unset - --edit will skip them.
                found_matches.push_back(MatchResult{ss.str(), std::nullopt});
            }
        }
    }

    return found_matches;
}
