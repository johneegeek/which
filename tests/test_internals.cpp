#include "internal_cmds.h"

#include <gtest/gtest.h>

class InternalCmdsSuite : public testing::TestWithParam<const char*> {};

TEST_P(InternalCmdsSuite, IsInternalCommand)
{
    ASSERT_TRUE(is_internal_command(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
    InternalCmds, InternalCmdsSuite,
    testing::Values("assoc", "break", "Call", "CD", "chdir", "cls", "color", "copy",
                    "DATE", "DEL", "DIR", "DPATH", "ECHO", "ENDLOCAL", "ERASE", "EXIT",
                    "FOR", "FTYPE", "GOTO", "IF", "KEYS", "MD", "MKDIR", "MKLINK",
                    "MOVE", "PATH", "PAUSE", "POPD", "PROMPT", "PUSHD", "REM", "REN",
                    "RENAME", "RD", "RMDIR", "SET", "SETLOCAL", "SHIFT", "START",
                    "TIME", "Title", "TYPE", "VER", "voL"));

TEST(InternalCmdsTest, Not_A_Command)
{
    ASSERT_FALSE(is_internal_command("foo"));
    ASSERT_FALSE(is_internal_command("bar"));
    ASSERT_FALSE(is_internal_command("baz"));
}