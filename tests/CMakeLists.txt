
enable_testing()
ADD_CUSTOM_TARGET(
        check
        ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure
)

find_package(GMock REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

pkg_check_modules(
    TEST_DEPENDENCIES
    libqtdbustest-1>=0.2 REQUIRED
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${TEST_DEPENDENCIES_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/include"
) 

add_subdirectory(functional)
add_subdirectory(utils)

add_definitions(
    -DMENU_DIR="${CMAKE_CURRENT_BINARY_DIR}/functional/menus"
)

add_executable(
    tests
    functional/FunctionalTests.cpp
    unit/TestMatchResult.cpp
)

target_link_libraries(
    tests
    test-main
    ${GMENU_HARNESS}
    ${TEST_DEPENDENCIES_LDFLAGS}
    Qt5::DBus
)