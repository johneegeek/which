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

struct PrettySize {
        std::uintmax_t size{};

    private:
        friend std::ostream& operator<<(std::ostream& os, PrettySize hr)
        {
            int    o{};
            double mantissa = hr.size;
            for (; mantissa >= 1024.; mantissa /= 1024., ++o)
                ;
            os << std::ceil(mantissa * 10.) / 10. << "BKMGTPE"[o];
            return o ? os << "B (" << hr.size << ')' : os;
        }
};

template<typename TP> std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                        + system_clock::now());
    return system_clock::to_time_t(sctp);
}

struct PrettyDateTime {
        std::filesystem::file_time_type ftime{};

    private:
        friend std::ostream& operator<<(std::ostream& os, PrettyDateTime pdt)
        {
            std::time_t ttime = to_time_t(pdt.ftime);
            std::tm*    gmt   = std::gmtime(&ttime);
            os << std::put_time(gmt, "%Y-%m-%d %H:%M:%S");
            return os;
        }
};

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

std::vector<std::filesystem::path> files_to_check(const std::string& filename)
{
    std::vector<std::filesystem::path> files;
    std::filesystem::path              path(filename);

    std::vector<std::string> extensions = get_path_exts();
    for (const auto& ext: extensions) { files.push_back(path.replace_extension(ext)); }

    return files;
}

std::vector<std::string> search_path(const std::string& filename,
                                     bool               with_size = false)
{
    std::vector<std::string> found_matches;

    std::vector<std::filesystem::path> check_list = files_to_check(filename);
    std::vector<std::filesystem::path> path_dirs  = get_path_dirs();
    for (const auto& search_path: path_dirs) {
        for (const auto& check: check_list) {
            std::filesystem::path findme = search_path / check;
            if (std::filesystem::exists(findme)) {
                std::stringstream ss;
                if (with_size) {
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