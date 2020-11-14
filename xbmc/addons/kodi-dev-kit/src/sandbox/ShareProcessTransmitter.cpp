/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ShareProcessTransmitter.h"
#include "SharedMemControlPosix.h"

namespace addon
{
namespace sandbox
{

CShareProcessTransmitter::CShareProcessTransmitter(const std::string& identifier, size_t size /* = SHARED_DEFAULT_MEM_SIZE*/)
  : m_identifier(identifier)
{
  m_memControl = std::make_shared<CSharedMemControlPosix>(identifier, size);
}

bool CShareProcessTransmitter::Create(bool initial)
{
  if (!m_memControl->Create(initial))
    return false;

  return true;
}

void CShareProcessTransmitter::Destroy()
{
  m_memControl->Destroy();
}

void CShareProcessTransmitter::SendMessage(const std::stringstream& in)
{
  std::unique_lock<std::mutex> lock(m_lock);

  apiShareData* sharedMem = m_memControl->GetSharedMem();

  int size = in.str().size();
  sharedMem->bigger = size > DEFAULT_SHARED_MEM_ARRAY_SIZE;
  sharedMem->data_size = sharedMem->bigger ? DEFAULT_SHARED_MEM_ARRAY_SIZE : size;
  memcpy(sharedMem->data, in.str().data(), sharedMem->data_size);
  m_memControl->Unlock_Target();
  m_memControl->Lock_Caller();
}

void CShareProcessTransmitter::SendMessage(const std::stringstream& in, std::stringstream& ret)
{
  std::unique_lock<std::mutex> lock(m_lock);

  apiShareData* sharedMem = m_memControl->GetSharedMem();

  int size = in.str().size();
  sharedMem->bigger = size > DEFAULT_SHARED_MEM_ARRAY_SIZE;
  sharedMem->data_size = sharedMem->bigger ? DEFAULT_SHARED_MEM_ARRAY_SIZE : size;
  memcpy(sharedMem->data, in.str().data(), sharedMem->data_size);
  m_memControl->Unlock_Target();
  m_memControl->Lock_Caller();
  ret.write(sharedMem->data, sharedMem->data_size);
}

} /* namespace sandbox */
} /* namespace addon */
