#ifndef WHICH_WHICH_H_
#define WHICH_WHICH_H_

#include <filesystem>
#include <vector>

extern std::vector<std::string> search_path(const std::string& filename,
                                            bool               with_size);

extern std::vector<std::filesystem::path> files_to_check(const std::string& filename);

extern std::vector<std::filesystem::path> get_path_dirs();

extern std::vector<std::string> get_path_exts();

#endif /* WHICH_WHICH_H_ */