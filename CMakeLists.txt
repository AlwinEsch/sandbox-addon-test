cmake_minimum_required(VERSION 3.5)
project(kodisandbox)

add_definitions(-DTARGET_LINUX -DTARGET_POSIX -DTEST_BUILD)

add_subdirectory(xbmc/addons/kodi-dev-kit/src)

add_executable(kodisandbox main_parent.cpp)
target_link_libraries(kodisandbox PUBLIC kodisandbox-server)

add_executable(kodichild main_child.cpp)
target_link_libraries(kodichild PUBLIC kodisandbox-child)
