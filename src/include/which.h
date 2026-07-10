/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_WHICH_H_
#define WHICH_WHICH_H_

#include <filesystem>
#include <string>
#include <vector>

#include "match_result.h"

extern std::vector<MatchResult> search_path(const std::string& filename,
                                            bool               show_info = false);

extern std::vector<std::filesystem::path> files_to_check(const std::string& filename);

extern std::vector<std::filesystem::path> get_path_dirs();

extern std::vector<std::string> get_path_exts();

/**
 * @brief Check if a file is one we're willing to hand to %EDITOR% for --edit.
 *
 * Based purely on extension (see which.cpp for the list). Anything not in
 * that list - including plain executables like .exe - is treated as
 * non-editable.
 *
 * @param file Path to check.
 * @return bool True if the file's extension is considered editable.
 */
extern bool is_editable_file(const std::filesystem::path& file);

#endif /* WHICH_WHICH_H_ */
