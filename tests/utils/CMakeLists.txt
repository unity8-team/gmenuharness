
add_library(
    test-main STATIC
    TestMain.cpp
)

target_link_libraries(
    test-main
    Qt5::Core
    ${GTEST_LIBRARIES}
)

