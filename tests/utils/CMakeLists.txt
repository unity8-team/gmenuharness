
add_library(
    test-main
    test-main.cpp
)

target_link_libraries(
    test-main
    Qt5::Core
    ${GTEST_LIBRARIES}
)

