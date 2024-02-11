#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
        std::filesystem::file_time_type ftime{};

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
            std::time_t ttime = to_time_t(pdt.ftime);
            std::tm*    gmt   = std::gmtime(&ttime);
            os << std::put_time(gmt, "%Y-%m-%d %H:%M:%S");
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
            int    o{};
            double mantissa = pfs.size;
            for (; mantissa >= 1024.; mantissa /= 1024., ++o)
                ;
            os << std::ceil(mantissa * 10.) / 10. << "BKMGTPE"[o];
            return o ? os << "B (" << pfs.size << ')' : os;
        }
};

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
    path_dirs.push_back("./");

    // Get the PATH from the environment
    const char* path = std::getenv("PATH");
    std::string path_s(path);

    std::vector<std::string> tokens;
    boost::split(tokens, path_s, boost::is_any_of(";"));

    for (const auto& token: tokens) {
        // Check if the token is a directory reduce checking bad paths.
        if (std::filesystem::is_directory(token)) { path_dirs.push_back(token); }
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

    std::vector<std::string> extensions = get_path_exts();
    for (const auto& ext: extensions) { files.push_back(path.replace_extension(ext)); }

    return files;
}

/**
 * @brief Search for a file in the PATH environment variable
 *
 * @param filename Name of the file to search for
 * @param show_info Show the size and date of the file
 * @return std::vector<std::string> A vector of strings containing all matching files
 */
std::vector<std::string> search_path(const std::string& filename,
                                     bool               show_info = false)
{
    std::vector<std::string> found_matches;

    std::vector<std::filesystem::path> check_list = files_to_check(filename);
    std::vector<std::filesystem::path> path_dirs  = get_path_dirs();
    for (const auto& search_path: path_dirs) {
        for (const auto& check: check_list) {
            std::filesystem::path findme = search_path / check;
            if (std::filesystem::exists(findme)) {
                std::stringstream ss;
                if (show_info) {
                    ss << PrettyDateTime{std::filesystem::last_write_time(findme)}
                       << "  " << PrettySize{std::filesystem::file_size(findme)}
                       << " \t ";
                }
                ss << findme.make_preferred().string();
                found_matches.push_back(ss.str());
            }
        }
    }

    return found_matches;
}