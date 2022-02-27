#include "SBomber.h"
#include "MyTools.h"
#include "ScreenSingleton.h"

#include <iostream>
#include <fstream>

#if defined(_WIN32) || defined(WIN32)
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#endif

int main(void) {
  MyTools::LoggerSingleton::getInstance().OpenLogFile("log.txt");

  SBomber game(std::make_unique<ConcreteCheckImpl>());

  do {
    game.TimeStart();

    game.ProcessKBHit(_kbhit());

    ScreenSingleton::getInstance().ClrScr();

    game.DrawFrame();
    game.MoveObjects();
    game.CheckObjects();
    game.RunCommands();
    game.TankMessage();

    game.TimeFinish();

  } while (!game.GetExitFlag());

  MyTools::LoggerSingleton::getInstance().CloseLogFile();

  return 0;
}
