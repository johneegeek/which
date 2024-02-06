include(GoogleTest)
enable_testing()

set( This Tests )

set( test_SRCS 
    test_internals.cpp
)

add_executable(${This} ${test_SRCS})
add_dependencies( ${This} whichlib)

target_link_libraries( ${This}
    whichlib 
    GTest::gtest_main
)

gtest_discover_tests( ${This} )