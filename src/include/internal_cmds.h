#ifndef WHICH_INTERNAL_CMDS_H_
#define WHICH_INTERNAL_CMDS_H_

#include <string>
#include <vector>

extern std::vector<std::string> search_internal_commands(const std::string& command);

extern bool is_internal_command(const std::string& command);

#endif // WHICH_INTERNAL_CMDS_H_
