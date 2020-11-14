/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ShareProcessReceiver.h"
#include "SharedMemControlPosix.h"

namespace addon
{
namespace sandbox
{

CShareProcessReceiver::CShareProcessReceiver(const std::string& identifier, size_t size /* = SHARED_DEFAULT_MEM_SIZE*/)
{
  m_memControl = std::make_shared<CSharedMemControlPosix>(identifier, size);
}

bool CShareProcessReceiver::Create(bool initial, bool useThread)
{
  if (!m_memControl->Create(initial))
    return false;

  m_running = true;
  if (useThread)
    m_thread = std::thread([&]{ Process(); });

  return true;
}

void CShareProcessReceiver::Destroy()
{
  m_running = false;
  m_memControl->Unlock_Target();

  if (m_thread.joinable())
  {
    m_thread.join();
  }

  m_memControl->Destroy();
}

void CShareProcessReceiver::RegisterReceiver(SandboxProcess func)
{
  m_receivers.emplace_back(func);
}

bool CShareProcessReceiver::HandleMessage(int func, const msgpack::unpacked& in, std::stringstream& out)
{
  bool ret = false;
  m_memControl->SetActive();

  out.str("");

  for (const auto& receiver : m_receivers)
  {
    if (receiver(func, in, out))
    {
      ret = true;
      break;
    }
  }

  m_memControl->SetInactive();

  return ret;
}

bool CShareProcessReceiver::ProcessOutside()
{
  std::stringstream ret;

  if (m_memControl->Lock_Target())
  {
    if (!m_running)
    {
      return false;
    }

    apiShareData* sharedMem = m_memControl->GetSharedMem();

    std::size_t offset = 0;
    msgpack::unpacked ident = msgpack::unpack(sharedMem->data, sharedMem->data_size, offset);
    int id = ident.get().as<decltype(id)>();

    auto input = msgpack::unpack(sharedMem->data, sharedMem->data_size, offset);
    if (!HandleMessage(id, input, ret))
    {
      m_memControl->Unlock_Caller();
      return true;
    }

    sharedMem->data_size = ret.str().size();
    memcpy(sharedMem->data, ret.str().data(), sharedMem->data_size);
  }

  m_memControl->Unlock_Caller();
  return true;
}


void CShareProcessReceiver::Process()
{
  while (m_running)
  {
    ProcessOutside();
  }
}

} /* namespace sandbox */
} /* namespace addon */
