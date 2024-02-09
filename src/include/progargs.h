#ifndef WHICH_PROGARGS_H_
#define WHICH_PROGARGS_H_

#include "cxxopts.hpp"

#include <memory>

extern cxxopts::ParseResult parse_args(int argc, char* argv[]);
extern void                 show_usage();

#endif /* WHICH_PROGARGS_H_ */