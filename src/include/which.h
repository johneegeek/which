#ifndef WHICH_WHICH_H_
#define WHICH_WHICH_H_

#include <filesystem>
#include <vector>

std::vector<std::string> search_path(const std::string& filename);

std::vector<std::filesystem::path> files_to_check(const std::string& filename);

std::vector<std::filesystem::path> get_path_dirs();

std::vector<std::string> get_path_exts();

#endif /* WHICH_WHICH_H_ */