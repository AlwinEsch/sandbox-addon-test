/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Launcher.h"

#include "utils/uuid.h"
#include "utils/Log.h"
#include "../sandbox/shared_process_func_child.h"
#include "../sandbox/shared_process_func_parent.h"

namespace addon
{
namespace sandbox
{

CChildLauncher::CChildLauncher()
  : m_identifier("kodi-sandproc-" + generateUUID()),
    m_mainTransmitter(std::make_shared<CShareProcessTransmitter>(m_identifier + "-receiver")),
    m_mainReceiver(std::make_shared<CShareProcessReceiver>(m_identifier))
{

}

bool CChildLauncher::StartObserver()
{
  m_running = true;
  m_thread = std::thread([&]{ ObserveProcess(); });

  m_mainReceiver->RegisterReceiver(std::bind(&CChildLauncher::HandleMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  return true;
}

bool CChildLauncher::StopObserver()
{
  m_running = false;

  if (GetLastChildStatus() == ChildStatus::Running)
  {
    std::stringstream in;
    msgpack::pack(in, int(funcIdChild::master_KillChild));
    msgpack::pack(in, msg_child_master_KillChild("end"));
    m_mainTransmitter->SendMessage(in);
  }

  if (m_thread.joinable())
    m_thread.join();

  return true;
}

bool CChildLauncher::HandleMessage(int func, const msgpack::unpacked& in, std::stringstream& out)
{
  switch (func)
  {
    case funcIdKodi::interface_CreateForNewThread:
    {
      msg_parent_interface_CreateForNewThread t = in.get().as<decltype(t)>();
      const std::string identifier = m_identifier + "-subthread-" + std::get<0>(t);

      m_otherThreadReceiver = std::make_shared<CShareProcessReceiver>(identifier);
      m_otherThreadReceiver->RegisterReceiver(std::bind(&CChildLauncher::HandleMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      if (!m_otherThreadReceiver->Create(true, true))
      {
        addon::utils::LOG_MESSAGE(ADDON_LOG_ERROR, "CProcessReceiver::%s: Failed to create other second receiver", __func__);
        return false;
      }

      msgpack::pack(out, identifier);
      break;
    }
    case funcIdKodi::kodi_Log:
    {
      msg_parent_kodi_Log t = in.get().as<decltype(t)>();
      addon::utils::LOG_MESSAGE(AddonLog(std::get<0>(t)), "CHILD: %s", std::get<1>(t).c_str());
      msgpack::pack(out, bool(true));
      break;
    }
    default:
      addon::utils::LOG_MESSAGE(ADDON_LOG_ERROR, "CProcessReceiver::%s: addon called with unknown function id '%i' on group 'kodi'", __func__, func);
      return false;
  }

  return true;
}

void CChildLauncher::ObserveProcess()
{
  while (m_running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    const ChildStatus status = ProcessActive();
    if (status == ChildStatus::Running)
    {

      continue;
    }
    else
    {

      break;
    }
  }
}

} /* namespace sandbox */
} /* namespace addon */
