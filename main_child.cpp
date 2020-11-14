#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "xbmc/addons/kodi-dev-kit/src/child/Processor.h"
#include "xbmc/addons/kodi-dev-kit/src/child/kodi/General.h"

namespace addon
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

} /* namespace addon */

int main(int argc, char* argv[])
{
  const std::string sandbox = addon::CheckSandbox(argc, argv);
  const std::string main_shared = addon::CheckMainShared(argc, argv);

  addon::sandbox::CChildProcessor processor(main_shared, true);

    addon::Log(ADDON_LOG_ERROR, "Hello World");

  std::thread thread = std::thread([&]{ addon::Log(ADDON_LOG_ERROR, "Hello another World"); });
  thread.detach();

  std::thread thread2 = std::thread([&]{ addon::Log(ADDON_LOG_ERROR, "Hello another World"); });
  thread2.detach();


  while (1)
  {
    if (!processor.ProcessOutside())
      break;
  }


  sleep(3);

  return 0;
}
