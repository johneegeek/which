/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_INTERNAL_CMDS_H_
#define WHICH_INTERNAL_CMDS_H_

#include <string>
#include <vector>

#include "match_result.h"

extern std::vector<MatchResult> search_internal_commands(const std::string& command);

extern bool is_internal_command(const std::string& command);

#endif // WHICH_INTERNAL_CMDS_H_
