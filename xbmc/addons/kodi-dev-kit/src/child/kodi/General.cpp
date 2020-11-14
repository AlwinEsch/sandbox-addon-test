/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "General.h"
#include "../Processor.h"
#include "../../sandbox/shared_process_func_parent.h"

#include <kodi/c-api/general.h>
#include <kodi/tools/StringUtils.h>
#include <stdio.h>

namespace addon
{

void Log(const AddonLog loglevel, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  const std::string str = kodi::tools::StringUtils::FormatV(format, args);
  va_end(args);

  std::stringstream in;
  msgpack::pack(in, int(funcIdKodi::kodi_Log));
  msgpack::pack(in, msg_parent_kodi_Log(loglevel, str));
  addon::sandbox::CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}
/*
std::string GetAddonPath(const std::string& append)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetAddonPath));
  msgpack::pack(in, stream_msg_kodi_GetAddonPath(append));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

std::string GetBaseUserPath(const std::string& append)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetBaseUserPath));
  msgpack::pack(in, stream_msg_kodi_GetBaseUserPath(append));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

std::string GetTempAddonPath(const std::string& append)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetTempAddonPath));
  msgpack::pack(in, stream_msg_kodi_GetTempAddonPath(append));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

std::string GetLocalizedString(uint32_t labelId, const std::string& defaultStr)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetLocalizedString));
  msgpack::pack(in, stream_msg_kodi_GetLocalizedString(labelId, defaultStr));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

std::string GetSettingString(const std::string& settingName, const std::string& defaultValue)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetSettingString));
  msgpack::pack(in, stream_msg_kodi_GetSettingString(settingName, defaultValue));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

void SetSettingString(const std::string& settingName, const std::string& settingValue)
{
  std::stringstream in;
  msgpack::pack(in, int(funcIdKodi::kodi_SetSettingString));
  msgpack::pack(in, stream_msg_kodi_SetSettingString(settingName, settingValue));
  client::share::CProcessChild::g_master->SendMessage(in);
}

int GetSettingInt(const std::string& settingName, int defaultValue)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetSettingInt));
  msgpack::pack(in, stream_msg_kodi_GetSettingInt(settingName, defaultValue));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<int>();
}

void SetSettingInt(const std::string& settingName, int settingValue)
{
  std::stringstream in;
  msgpack::pack(in, int(funcIdKodi::kodi_SetSettingInt));
  msgpack::pack(in, stream_msg_kodi_SetSettingInt(settingName, settingValue));
  client::share::CProcessChild::g_master->SendMessage(in);
}

bool GetSettingBoolean(const std::string& settingName, bool defaultValue)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetSettingBoolean));
  msgpack::pack(in, stream_msg_kodi_GetSettingBoolean(settingName, defaultValue));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<bool>();
}

void SetSettingBoolean(const std::string& settingName, bool settingValue)
{
  std::stringstream in;
  msgpack::pack(in, int(funcIdKodi::kodi_SetSettingBoolean));
  msgpack::pack(in, stream_msg_kodi_SetSettingBoolean(settingName, settingValue));
  client::share::CProcessChild::g_master->SendMessage(in);
}

std::string GetLanguage(LangFormats format, bool region)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetLanguage));
  msgpack::pack(in, stream_msg_kodi_GetLanguage(uint32_t(format), region));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}

void QueueNotification(QueueMsg type,
                       const std::string& header,
                       const std::string& message,
                       const std::string& imageFile,
                       unsigned int displayTime,
                       bool withSound,
                       unsigned int messageTime)
{
  std::stringstream in;
  msgpack::pack(in, int(funcIdKodi::kodi_QueueNotification));
  msgpack::pack(in, stream_msg_kodi_QueueNotification(uint32_t(type), header, message, imageFile, displayTime, withSound, messageTime));
  client::share::CProcessChild::g_master->SendMessage(in);
}

std::string GetAddonInfo(const std::string& id)
{
  std::size_t offset = 0;
  std::stringstream in;
  std::stringstream out;
  msgpack::pack(in, int(funcIdKodi::kodi_GetAddonInfo));
  msgpack::pack(in, stream_msg_kodi_GetAddonInfo(id));
  client::share::CProcessChild::g_master->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.str().data(), out.str().size(), offset);
  return ident.get().as<std::string>();
}*/

} /* namespace addon */
