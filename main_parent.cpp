#include "xbmc/addons/kodi-dev-kit/src/server/LauncherPosix.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  addon::sandbox::CChildLauncherPosix launcher;

  std::vector<std::string> arg;
  arg.push_back("./kodichild");
  arg.push_back("--sandbox=webbrowser");
  launcher.Launch(arg);

//   int wait = 15;
//   while (wait-- > 0)
//   {
//     const auto before_fork = std::chrono::high_resolution_clock::now();
//     launcher.ProcessActive();
//     const auto after_fork = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> fork_time = std::chrono::duration_cast<std::chrono::duration<double>>(after_fork - before_fork);
//     addon::utils::LOG_MESSAGE(ADDON_LOG_DEBUG, "%s: time %f", __func__, fork_time.count());
//     sleep(1);
//   }

  sleep(15);

  launcher.Kill(true);

  return 0;
}
