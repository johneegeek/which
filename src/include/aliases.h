/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_ALIASES_H_
#define WHICH_ALIASES_H_

#include <string>
#include <vector>

#include "match_result.h"

extern std::vector<MatchResult> search_aliases(const std::string& command);

#endif /* WHICH_ALIASES_H_ */
