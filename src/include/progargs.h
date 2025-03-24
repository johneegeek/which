/******************************************************************************
 * Copyright (c) 2018-2025 John Kiernan
 *
 * `Which` is licensed under MIT license,
 *  see https://opensource.org/licenses/MIT
 ******************************************************************************/

#ifndef WHICH_PROGARGS_H_
#define WHICH_PROGARGS_H_

#include "cxxopts.hpp"

#include <memory>

extern cxxopts::ParseResult parse_args(int argc, char* argv[]);
extern void                 show_usage();

#endif /* WHICH_PROGARGS_H_ */