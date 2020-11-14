/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <kodi/c-api/addon_base.h>
#include <string>

namespace addon
{

typedef enum LangFormats
{
  LANG_FMT_ISO_639_1,
  LANG_FMT_ISO_639_2,
  LANG_FMT_ENGLISH_NAME
} LangFormats;

typedef enum QueueMsg
{
  QUEUE_INFO,
  QUEUE_WARNING,
  QUEUE_ERROR,
  QUEUE_OWN_STYLE
} QueueMsg;

void Log(const AddonLog loglevel, const char* format, ...);
std::string GetAddonPath(const std::string& append = "");
std::string GetBaseUserPath(const std::string& append = "");
std::string GetTempAddonPath(const std::string& append = "");
std::string GetLocalizedString(uint32_t labelId, const std::string& defaultStr = "");
std::string GetSettingString(const std::string& settingName, const std::string& defaultValue = "");
void SetSettingString(const std::string& settingName, const std::string& settingValue);
int GetSettingInt(const std::string& settingName, int defaultValue = 0);
void SetSettingInt(const std::string& settingName, int settingValue);
bool GetSettingBoolean(const std::string& settingName, bool defaultValue = false);
void SetSettingBoolean(const std::string& settingName, bool settingValue);
std::string GetLanguage(LangFormats format = LANG_FMT_ENGLISH_NAME, bool region = false);
void QueueNotification(QueueMsg type,
                       const std::string& header,
                       const std::string& message,
                       const std::string& imageFile = "",
                       unsigned int displayTime = 5000,
                       bool withSound = true,
                       unsigned int messageTime = 1000);
std::string GetAddonInfo(const std::string& id);

} /* namespace addon */
