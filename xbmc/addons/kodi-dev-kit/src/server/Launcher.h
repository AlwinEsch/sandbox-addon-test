/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../sandbox/ShareProcessTransmitter.h"
#include "../sandbox/ShareProcessReceiver.h"

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace addon
{
namespace sandbox
{

enum class ChildStatus
{
  Running,
  NotStarted,
  ExitedNormal,
  StoppedBySignal,
  StoppedByUnknown,
  KilledFromOutside,
  SeqmentionFault,
};

class CChildLauncher
{
public:
  CChildLauncher();
  virtual ~CChildLauncher() = default;

  const std::string& GetIdentifier() const { return m_identifier; }

  virtual bool Launch(const std::vector<std::string>& argv, bool waitForExit) = 0;
  virtual ChildStatus ProcessActive() = 0;
  virtual bool Kill(bool wait = true) = 0;

  ChildStatus GetLastChildStatus() const { return m_lastStatus;  }
  int GetLExitCode() const { return m_exitCode;  }

protected:
  bool StartObserver();
  bool StopObserver();

  const std::string m_identifier;

  std::shared_ptr<CShareProcessTransmitter> m_mainTransmitter;
  std::shared_ptr<CShareProcessReceiver> m_mainReceiver;
  std::shared_ptr<CShareProcessReceiver> m_otherThreadReceiver;
  std::atomic<ChildStatus> m_lastStatus{ChildStatus::NotStarted};
  int m_exitCode = 0;

private:
  void ObserveProcess();

  bool HandleMessage(int func, const msgpack::unpacked& in, std::stringstream& out);

  std::atomic_bool m_running{false};
  std::thread m_thread;
};

} /* namespace sandbox */
} /* namespace addon */
