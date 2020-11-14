/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Processor.h"
#include "../sandbox/shared_process_func_child.h"
#include "../sandbox/shared_process_func_parent.h"
#include "kodi/General.h"

namespace addon
{
namespace sandbox
{

std::string CheckSandbox(int argc, char* argv[])
{
  const char* value = "--sandbox=";
  std::string sandbox;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == nullptr)
      break;

    std::string arg = argv[i];
    if (arg.rfind(value, 0) == 0)
    {
      sandbox = arg.substr(strlen(value));
      break;
    }
    else if ((arg == "-r") || (arg == "--sandbox"))
    {
      if (i + 1 < argc)  // Make sure we aren't at the end of argv!
      {
        sandbox = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
        break;
      }
    }
  }

  if (sandbox.empty())
  {
    fprintf(stderr, "--sandbox option not set\n");
  }
  return sandbox;
}

std::string CheckMainShared(int argc, char* argv[])
{
  const char* value = "--main-shared=";
  std::string main_shared;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == nullptr)
      break;

    std::string arg = argv[i];
    if (arg.rfind(value, 0) == 0)
    {
      main_shared = arg.substr(strlen(value));
      break;
    }
    else if ((arg == "-m") || (arg == "--main-shared"))
    {
      if (i + 1 < argc)  // Make sure we aren't at the end of argv!
      {
        main_shared = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
        break;
      }
    }
  }

  if (main_shared.empty())
  {
    fprintf(stderr, "--main-shared option not set\n");
  }
  return main_shared;
}

std::thread::id main_thread_id = std::this_thread::get_id();
std::shared_ptr<addon::sandbox::CShareProcessReceiver> CChildProcessor::g_mainIn;
std::shared_ptr<addon::sandbox::CShareProcessTransmitter> CChildProcessor::g_mainOut;
std::shared_ptr<addon::sandbox::CShareProcessTransmitter> CChildProcessor::g_other;
std::mutex CChildProcessor::g_lock;

CChildProcessor::CChildProcessor(const std::string& main_shared, bool viaMainThread)
  : m_mainShared(main_shared)
{
  std::shared_ptr<addon::sandbox::CShareProcessTransmitter> transmitter = std::make_shared<addon::sandbox::CShareProcessTransmitter>(m_mainShared);
  if (!transmitter->Create(false))
  {
    fprintf(stderr, "FATAL: Failed to init main transmit process of sandbox, process not usable!\n");
    exit(EXIT_FAILURE);
  }

  g_mainOut = transmitter;

  std::shared_ptr<addon::sandbox::CShareProcessReceiver> receiver = std::make_shared<addon::sandbox::CShareProcessReceiver>(m_mainShared + "-receiver");
  receiver->RegisterReceiver(std::bind(&CChildProcessor::HandleMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  if (!receiver->Create(false, !viaMainThread))
  {
    fprintf(stderr, "FATAL: Failed to init main receive process of sandbox, process not usable!\n");
    exit(EXIT_FAILURE);
  }

  g_mainIn = receiver;

  addon::Log(ADDON_LOG_INFO, "Inform for start of sandbox process id '%s'", m_mainShared.c_str());
}

CChildProcessor::~CChildProcessor()
{
  g_mainOut.reset();
  g_other.reset();
}

bool CChildProcessor::ProcessOutside()
{
  if (!g_mainIn)
    return false;

  return g_mainIn->ProcessOutside();
}

std::shared_ptr<addon::sandbox::CShareProcessTransmitter> CChildProcessor::GetCurrentProcessor()
{
  std::thread::id threadId = std::this_thread::get_id();

  if (main_thread_id == threadId)
    return g_mainOut;

  std::unique_lock<std::mutex> lock(g_lock);

  if (!g_other)
  {
    std::size_t offset = 0;
    std::stringstream in;
    std::stringstream out;
    msgpack::pack(in, int(funcIdKodi::interface_CreateForNewThread));
    msgpack::pack(in, msg_parent_interface_CreateForNewThread("other"));
    g_mainOut->SendMessage(in, out);
    const std::string ident = msgpack::unpack(out.str().data(), out.str().size(), offset).get().as<std::string>();
    std::shared_ptr<addon::sandbox::CShareProcessTransmitter> transmitter = std::make_shared<addon::sandbox::CShareProcessTransmitter>(ident);
    if (!transmitter->Create(false))
    {
      fprintf(stderr, "FATAL: Failed to init other process of sandbox, process not usable!\n");
      return g_mainOut;
    }

    g_other = transmitter;
  }

  return g_other;
}

bool CChildProcessor::HandleMessage(int func, const msgpack::unpacked& in, std::stringstream& out)
{
  switch (func)
  {
    case funcIdChild::master_KillChild:
    {
      addon::Log(ADDON_LOG_INFO, "Informed for stop of sandbox process id '%s'", m_mainShared.c_str());
      return false;
      break;
    }
    default:
      addon::Log(ADDON_LOG_FATAL, "CChildProcessor::%s: addon called with unknown function id '%i'", __func__, func);
      return false;
  }

  return true;
}


} /* namespace sandbox */
} /* namespace addon */
