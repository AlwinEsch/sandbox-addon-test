/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "SharedMemControl.h"

namespace addon
{
namespace sandbox
{

CSharedMemControl::CSharedMemControl(const std::string& identifier,
                                     size_t size /* = DEFAULT_SHARED_MEM_SIZE*/)
  : m_identifier(identifier),
    m_sharedMemSize(size)
{
}

void CSharedMemControl::SetActive()
{
  if (!m_sharedMem)
    return;

  m_sharedMem->sem_segment_active++;

  if (m_sharedMem->sem_segment_active == SHARED_MAX_CALL_AMOUNT)
  {
    fprintf(stderr, "FATAL: Shared mem intercalls exceded maximum amount of '%i', crash coming\n", SHARED_MAX_CALL_AMOUNT);
    return;
  }

  if (!m_sharedMem->sem_segments[m_sharedMem->sem_segment_active].inited)
  {
    InitSegment(m_sharedMem->sem_segment_active);
  }
}

void CSharedMemControl::SetInactive()
{
  if (!m_sharedMem)
    return;

  if (m_sharedMem->sem_segment_active > 0)
    m_sharedMem->sem_segment_active--;
}

} /* namespace sandbox */
} /* namespace addon */
