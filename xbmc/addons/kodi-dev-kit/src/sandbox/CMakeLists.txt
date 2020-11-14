add_library(devkit_sandbox INTERFACE)

# cmake_policy(SET CMP0076 NEW)


set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosix.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessReceiver.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessTransmitter.cpp)
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/shared_data_structure.h
            ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosix.h
            ${CMAKE_CURRENT_SOURCE_DIR}/shared_process_func_child.h
            ${CMAKE_CURRENT_SOURCE_DIR}/shared_process_func_parent.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessReceiver.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ShareProcessTransmitter.h)

set(SOURCES_POSIX ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosix.cpp)
set(HEADERS_POSIX ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlPosix.h)

set(SOURCES_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlWindows.cpp)
set(HEADERS_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/SharedMemControlWindows.h)

target_sources(devkit_sandbox INTERFACE ${SOURCES}
                                        ${HEADERS})

if (WIN32 OR WINDOWS_STORE)
  target_sources(devkit_sandbox INTERFACE ${SOURCES_WINDOWS})
  target_sources(devkit_sandbox INTERFACE ${HEADERS_WINDOWS})
else()
  target_sources(devkit_sandbox INTERFACE ${SOURCES_POSIX})
  target_sources(devkit_sandbox INTERFACE ${HEADERS_POSIX})
endif()
