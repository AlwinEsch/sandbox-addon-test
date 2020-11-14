/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "SharedMemControl.h"

namespace addon
{
namespace sandbox
{

class CSharedMemControlPosix : public CSharedMemControl
{
public:
  CSharedMemControlPosix(const std::string& identifier, size_t size = SHARED_DEFAULT_MEM_SIZE);

  bool Create(bool initial) override;
  bool Destroy() override;

  bool Lock_Caller() override;
  void Unlock_Caller() override;

  bool Lock_Target() override;
  void Unlock_Target() override;

  bool InitSegment(int segment) override;

private:
  int m_fd{-1};
};

} /* namespace sandbox */
} /* namespace addon */
