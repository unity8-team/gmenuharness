project(gmenu-harness C CXX)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")
find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ${COMMON_FLAGS} -fno-strict-aliasing -fvisibility-inlines-hidden")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# version
# Read API versions. from VERSION file. The version is in the form <major>.<minor>.<micro>.

file(READ debian/VERSION GMENUHARNESS_FULL_VERSION)
string(STRIP ${GMENUHARNESS_FULL_VERSION} GMENUHARNESS_FULL_VERSION)

# Parse out the three components.
STRING(REGEX MATCHALL "[0-9]+" full_version_components ${GMENUHARNESS_FULL_VERSION})
LIST(GET full_version_components 0 GMENU_HARNESS_SO_VERSION_MAJOR)
LIST(GET full_version_components 1 GMENU_HARNESS_SO_VERSION_MINOR)
LIST(GET full_version_components 2 GMENU_HARNESS_SO_VERSION_MICRO)

set(GMENU_HARNESS gmenuharness)
set(GMENU_HARNESS_SO_VERSION ${GMENU_HARNESS_SO_VERSION_MAJOR}.${GMENU_HARNESS_SO_VERSION_MINOR}.${GMENU_HARNESS_SO_VERSION_MICRO})
set(GMENU_HARNESS_SO_VERSION_MAJOR_MINOR ${GMENU_HARNESS_SO_VERSION_MAJOR}.${GMENU_HARNESS_SO_VERSION_MINOR})
set(GMENU_HARNESS_HDR_INSTALL_BASE_DIR include/${GMENU_HARNESS}-${GMENU_HARNESS_SO_VERSION_MAJOR_MINOR})
set(GMENU_HARNESS_HDR_INSTALL_DIR ${GMENU_HARNESS_HDR_INSTALL_BASE_DIR}/unity/${GMENU_HARNESS})

include_directories(include)

set(GLIB_REQUIRED_VERSION 2.26)

pkg_check_modules(
  GLIB REQUIRED
  glib-2.0>=${GLIB_REQUIRED_VERSION}
  gio-2.0>=${GLIB_REQUIRED_VERSION}
)
include_directories(${GLIB_INCLUDE_DIRS})

pkg_check_modules(
  UNITY_API REQUIRED
  libunity-api
)
include_directories(${UNITY_API_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(include)
