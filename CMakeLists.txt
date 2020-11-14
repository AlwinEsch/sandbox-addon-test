cmake_minimum_required(VERSION 3.5)
project(kodisandbox)

add_definitions(-DTARGET_LINUX -DTARGET_POSIX -DTEST_BUILD)

set(SOURCES main_parent.cpp
            xbmc/addons/kodi-dev-kit/src/server/Launcher.cpp
            xbmc/addons/kodi-dev-kit/src/server/LauncherPosix.cpp
            xbmc/addons/kodi-dev-kit/src/server/utils/Log.cpp
            xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControl.cpp
            xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControlPosix.cpp
            xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessReceiver.cpp
            xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessTransmitter.cpp)

set(HEADERS xbmc/addons/kodi-dev-kit/src/server/Launcher.h
            xbmc/addons/kodi-dev-kit/src/server/LauncherPosix.h
            xbmc/addons/kodi-dev-kit/src/server/utils/Log.h
            xbmc/addons/kodi-dev-kit/src/server/utils/Utils.h
            xbmc/addons/kodi-dev-kit/src/server/utils/uuid.h
            xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControl.h
            xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControlPosix.h
            xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessReceiver.h
            xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessTransmitter.h)

add_executable(kodisandbox ${SOURCES}
                           ${HEADERS})
target_include_directories(kodisandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(kodisandbox pthread rt)


set(SOURCES_CHILD main_child.cpp
                  xbmc/addons/kodi-dev-kit/src/child/Processor.cpp
                  xbmc/addons/kodi-dev-kit/src/child/kodi/General.cpp
                  xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControl.cpp
                  xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControlPosix.cpp
                  xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessReceiver.cpp
                  xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessTransmitter.cpp)

set(HEADERS_CHILD xbmc/addons/kodi-dev-kit/src/child/Processor.h
                  xbmc/addons/kodi-dev-kit/src/child/kodi/General.h
                  xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControl.h
                  xbmc/addons/kodi-dev-kit/src/sandbox/SharedMemControlPosix.h
                  xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessReceiver.h
                  xbmc/addons/kodi-dev-kit/src/sandbox/ShareProcessTransmitter.h)

add_executable(kodichild ${SOURCES_CHILD}
                         ${HEADERS_CHILD})
target_include_directories(kodichild PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(kodichild pthread rt)


