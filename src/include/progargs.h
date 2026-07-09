/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_PROGARGS_H_
#define WHICH_PROGARGS_H_

#include "cxxopts.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
// argv mirrors main()'s required signature; it cannot be changed to a
// container type at this boundary. See progargs.cpp for the definition.
extern cxxopts::ParseResult parse_args(int argc, char* argv[]);
// NOLINTEND(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
extern void                 show_usage();

#endif /* WHICH_PROGARGS_H_ */