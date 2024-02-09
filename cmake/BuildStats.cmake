include_guard()

message(STATUS "Collecting build stats...")

string(TIMESTAMP BUILD_TIMESTAMP +%y%j%H)

if (PROJECT_USES_GIT)
    execute_process(COMMAND git log -1 --pretty=format:%h
                    OUTPUT_VARIABLE GIT_SHA
    )
endif()