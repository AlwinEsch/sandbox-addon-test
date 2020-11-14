set(SOURCES Launcher.cpp
            LauncherPosix.cpp
            utils/Log.cpp)

set(HEADERS Launcher.h
            LauncherPosix.h
            utils/Log.h
            utils/Utils.h
            utils/uuid.h)

set(SOURCES_POSIX LauncherPosix.cpp)

set(HEADERS_POSIX LauncherPosix.h)

add_library(kodisandbox-server STATIC ${SOURCES}
                                      ${HEADERS})

target_link_libraries(kodisandbox-server devkit_sandbox)

if (WIN32 OR WINDOWS_STORE)
  target_sources(kodisandbox-server PRIVATE ${SOURCES_WINDOWS})
  target_sources(kodisandbox-server PRIVATE ${HEADERS_WINDOWS})
  target_link_libraries(kodisandbox-server )
else()
  target_sources(kodisandbox-server PRIVATE ${SOURCES_POSIX})
  target_sources(kodisandbox-server PRIVATE ${HEADERS_POSIX})
  target_link_libraries(kodisandbox-server pthread rt)
endif()
