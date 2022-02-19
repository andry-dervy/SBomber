#include "Tower.h"

using namespace MyTools;

bool Tower::isInRange(double x1, double x2) const
{
  const double XBeg = x + 2;
  const double XEnd = x + width - 1;

  if (x1 < XBeg && x2 > XEnd)
  {
    return true;
  }

  if (x1 > XBeg && x1 < XEnd)
  {
    return true;
  }

  if (x2 > XBeg && x2 < XEnd)
  {
    return true;
  }

  return false;
}

void Tower::Paint() const
{
  ScreenSingleton::getInstance().SetColor(CC_Cyan);
  ScreenSingleton::getInstance().GotoXY(x, y - 5);
  std::cout << "######";
  ScreenSingleton::getInstance().GotoXY(x, y - 4);
  std::cout << "######";
  ScreenSingleton::getInstance().GotoXY(x, y - 3);
  std::cout << "######";
  ScreenSingleton::getInstance().GotoXY(x, y - 2);
  std::cout << "##  ##";
  ScreenSingleton::getInstance().GotoXY(x, y - 1);
  std::cout << "##  ##";
  ScreenSingleton::getInstance().GotoXY(x, y);
  std::cout << "##  ##";
}

bool TowerAdapter::isInside(double x1, double x2) const
{
  return tower->isInRange(x1,x2);
}

void TowerAdapter::Draw() const
{
  tower->Paint();
}

void TowerAdapter::SetWidth(uint16_t widthN)
{
  tower->SetWidth(widthN);
}

void TowerAdapter::SetPos(double nx, double ny)
{
  tower->SetPos(nx,ny);
}

std::string TowerAdapter::ClassID() const {
  return "Tower";
}
