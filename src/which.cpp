#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

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

std::vector<std::string> search_path(const std::string& filename)
{
    std::vector<std::string> found_matches;

    std::vector<std::filesystem::path> check_list = files_to_check(filename);
    std::vector<std::filesystem::path> path_dirs  = get_path_dirs();
    for (const auto& search_path: path_dirs) {
        for (const auto& check: check_list) {
            std::filesystem::path findme = search_path / check;
            if (std::filesystem::exists(findme)) {
                found_matches.push_back(findme.make_preferred().string());
            }
        }
    }

    return found_matches;
}