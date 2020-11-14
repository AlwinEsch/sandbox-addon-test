/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "shared_data_structure.h"

#include <string>

namespace addon
{
namespace sandbox
{

class CSharedMemControl
{
public:
  CSharedMemControl(const std::string& identifier, size_t size = SHARED_DEFAULT_MEM_SIZE);
  virtual ~CSharedMemControl() = default;

  const std::string& GetIdentifier() const { return m_identifier; }
  apiShareData* GetSharedMem() { return m_sharedMem; }
  size_t GetSharedMemSize() const { return m_sharedMemSize; }
  const std::string& GetLastErrorText() const { return m_errorText; }

  virtual bool Create(bool initial) = 0;
  virtual bool Destroy() = 0;

  virtual bool Lock_Caller() = 0;
  virtual void Unlock_Caller() = 0;

  virtual bool Lock_Target() = 0;
  virtual void Unlock_Target() = 0;

  virtual bool InitSegment(int segment) = 0;

  void SetActive();
  void SetInactive();

protected:
  apiShareData* m_sharedMem = nullptr;
  std::string m_errorText;

private:
  const std::string m_identifier;
  const size_t m_sharedMemSize;
};

} /* namespace sandbox */
} /* namespace addon */
