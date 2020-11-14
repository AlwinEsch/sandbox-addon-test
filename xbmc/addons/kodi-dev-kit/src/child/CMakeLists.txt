set(SOURCES Processor.cpp
            kodi/General.cpp)

set(HEADERS Processor.h
            kodi/General.h)

add_library(kodisandbox-child STATIC ${SOURCES}
                                      ${HEADERS})

target_link_libraries(kodisandbox-child devkit_sandbox)

if (WIN32 OR WINDOWS_STORE)
  target_sources(kodisandbox-child PRIVATE ${SOURCES_WINDOWS})
  target_sources(kodisandbox-child PRIVATE ${HEADERS_WINDOWS})
  target_link_libraries(kodisandbox-child )
else()
  target_sources(kodisandbox-child PRIVATE ${SOURCES_POSIX})
  target_sources(kodisandbox-child PRIVATE ${HEADERS_POSIX})
  target_link_libraries(kodisandbox-child devkit_sandbox pthread rt)
endif()
