#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "xbmc/addons/kodi-dev-kit/src/child/Processor.h"
#include "xbmc/addons/kodi-dev-kit/src/child/kodi/General.h"

int main(int argc, char* argv[])
{
  const std::string sandbox = addon::sandbox::CheckSandbox(argc, argv);
  const std::string main_shared = addon::sandbox::CheckMainShared(argc, argv);
  if (sandbox.empty() || main_shared.empty())
  {
    fprintf(stderr, "%s called independent, this is a sandbox only and started by Kodi itself!\n", argv[0]);
    return EXIT_FAILURE;
  }

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

  return EXIT_SUCCESS;
}
