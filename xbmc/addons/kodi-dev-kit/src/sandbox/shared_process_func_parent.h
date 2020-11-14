/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <stdint.h>
#include <string>
#include <tuple>

typedef enum funcIdKodi : int
{
  interface_CreateForNewThread,
  kodi_Log,
} funcIdKodi;

typedef std::tuple<std::string> msg_parent_interface_CreateForNewThread;
typedef std::tuple<int, std::string> msg_parent_kodi_Log;
