cmake_minimum_required(VERSION 3.31)
project(gosdclock)

find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(include)
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(gosdclock
  src/config.cpp
  src/gosd_clock.cpp
  src/osd_window.cpp
  src/settings_window.cpp
)
target_link_libraries(gosdclock ${GTKMM_LIBRARIES} X11 Xext)

add_executable(gosdclock_settings
  src/config.cpp
  src/gosd_settings.cpp
  src/settings_window.cpp
)
target_link_libraries(gosdclock_settings ${GTKMM_LIBRARIES})
