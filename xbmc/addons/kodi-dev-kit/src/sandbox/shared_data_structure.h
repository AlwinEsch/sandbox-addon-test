/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <functional>
#include <msgpack.hpp>

#if (defined TARGET_WINDOWS)
typedef HANDLE SEMAPHORE_ID;
typedef unsigned long THREAD_ID;
#elif (defined TARGET_POSIX)
#include <semaphore.h>

typedef sem_t SEMAPHORE_ID;
typedef pthread_t THREAD_ID;
#else
#error Not supported OS used
#endif

#define SHARED_MAX_CALL_AMOUNT 10
#define SHARED_DEFAULT_MEM_SIZE 10 * 1024
#define DEFAULT_SHARED_MEM_ARRAY_SIZE SHARED_DEFAULT_MEM_SIZE - (sizeof(uint32_t) * 10)

typedef struct apiShareData
{
  struct
  {
    bool inited;
    SEMAPHORE_ID sem_segment_caller;
    SEMAPHORE_ID sem_segment_target;
  } sem_segments[SHARED_MAX_CALL_AMOUNT];
  int sem_segment_active;

  THREAD_ID calling_thread;

  bool bigger;
  size_t data_size;
  char data[SHARED_DEFAULT_MEM_SIZE];
} apiShareData;

using SandboxProcess = std::function<bool(int func, const msgpack::unpacked& in, std::stringstream& out)>;
