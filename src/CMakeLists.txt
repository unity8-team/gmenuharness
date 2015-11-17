pkg_check_modules(UNITY_API libunity-api>=0.1.3 REQUIRED)
include_directories(${UNITY_API_INCLUDE_DIRS})

add_library(
    ${GMENU_HARNESS} SHARED
    MatchResult.cpp
    MatchUtils.cpp
    MenuItemMatcher.cpp
    MenuMatcher.cpp
)

set_target_properties(${GMENU_HARNESS} PROPERTIES
    VERSION "${GMENU_HARNESS_SO_VERSION_MAJOR}.${GMENU_HARNESS_SO_VERSION_MINOR}.${GMENU_HARNESS_SO_VERSION_MICRO}"
    SOVERSION ${GMENU_HARNESS_SO_VERSION_MAJOR_MINOR}
)


set(symbol_map "${CMAKE_SOURCE_DIR}/src/libgmenuharness.map")
set_target_properties(${GMENU_HARNESS} PROPERTIES
                      LINK_FLAGS "${ldflags} -Wl,--version-script,${symbol_map} ")
set_target_properties(${GMENU_HARNESS} PROPERTIES LINK_DEPENDS ${symbol_map})

target_link_libraries(
    gmenuharness
    ${GLIB_LDFLAGS}
)

install(
  TARGETS ${GMENU_HARNESS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
)

# Set up package config.
configure_file(lib${GMENU_HARNESS}.pc.in lib${GMENU_HARNESS}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${GMENU_HARNESS}.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig)