#include "../Classes/AppDelegate.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

USING_NS_CC;

int main(int argc, char **argv)
{

  bool bIsServer = true;
  std::string sFilename;
  if (argc >= 2)
  {
    bIsServer = std::string(argv[1]) == "client" ? false : true;
  }
  if (argc >= 3)
  {
    sFilename = argv[2];
  }

  // create the application instance
  AppDelegate app(bIsServer, sFilename);
  return Application::getInstance()->run();
}
