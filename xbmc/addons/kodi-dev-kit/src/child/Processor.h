/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../sandbox/ShareProcessTransmitter.h"
#include "../sandbox/ShareProcessReceiver.h"

#include <map>
#include <mutex>

namespace addon
{
namespace sandbox
{

class CChildProcessor
{
public:
  CChildProcessor(const std::string& main_shared, bool viaMainThread);
  virtual ~CChildProcessor();

  bool ProcessOutside();

  static std::shared_ptr<addon::sandbox::CShareProcessTransmitter> GetCurrentProcessor();

protected:
  bool HandleMessage(int func, const msgpack::unpacked& in, std::stringstream& out);

private:
  static std::shared_ptr<addon::sandbox::CShareProcessReceiver> g_mainIn;
  static std::shared_ptr<addon::sandbox::CShareProcessTransmitter> g_mainOut;
  static std::shared_ptr<addon::sandbox::CShareProcessTransmitter> g_other;
  static std::mutex g_lock;

  const std::string m_mainShared;
};

} /* namespace sandbox */
} /* namespace addon */
