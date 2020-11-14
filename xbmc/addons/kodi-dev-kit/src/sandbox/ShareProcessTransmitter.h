/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "shared_data_structure.h"

#include <atomic>
#include <memory>
#include <msgpack.hpp>
#include <mutex>
#include <string>
#include <thread>

namespace addon
{
namespace sandbox
{

class CSharedMemControl;

class CShareProcessTransmitter
{
public:
  CShareProcessTransmitter(const std::string& identifier, size_t size = SHARED_DEFAULT_MEM_SIZE);
  ~CShareProcessTransmitter() = default;

  const std::string& GetIdentifier() const { return m_identifier; }

  bool Create(bool initial);
  void Destroy();

  void SendMessage(const std::stringstream& in);
  void SendMessage(const std::stringstream& in, std::stringstream& ret);

private:
  const std::string m_identifier;
  std::mutex m_lock;
  std::shared_ptr<CSharedMemControl> m_memControl;
};

} /* namespace sandbox */
} /* namespace addon */
