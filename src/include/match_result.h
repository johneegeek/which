/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_MATCH_RESULT_H_
#define WHICH_MATCH_RESULT_H_

#include <filesystem>
#include <optional>
#include <string>

/**
 * @brief A single match returned by search_aliases(), search_internal_commands(),
 * or search_path().
 *
 * `display` is the human readable line printed to the user.
 *
 * `path` is only set when the match corresponds to a real file found via the
 * PATH search (search_path()); it is unset for aliases and internal commands,
 * since those aren't files on disk. This is what --edit uses to decide
 * whether there is anything sensible to hand to %EDITOR%.
 */
struct MatchResult {
        std::string                          display;
        std::optional<std::filesystem::path> path;
};

#endif // WHICH_MATCH_RESULT_H_
