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

typedef enum funcIdChild : int
{
  master_KillChild,
} funcIdChild;

typedef std::tuple<std::string> msg_child_master_KillChild;
