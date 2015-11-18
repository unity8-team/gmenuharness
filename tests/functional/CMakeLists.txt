
add_subdirectory(menus)
add_definitions(
    -DMENU_DIR="${CMAKE_CURRENT_BINARY_DIR}/menus"
)

add_executable(
    functional-tests
    functional-tests.cpp
)

target_link_libraries(
    functional-tests
    test-main
    ${GMENU_HARNESS}
    ${TEST_DEPENDENCIES_LDFLAGS}
    Qt5::DBus
)

add_test(
    functional-tests
    functional-tests
)
