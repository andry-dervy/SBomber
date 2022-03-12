
#include <iostream>

#include "Plane.h"
#include "MyTools.h"
#include "ScreenSingleton.h"

void Plane::Draw() const
{
    DrawBody();
    DrawWings();
    DrawTail();
    ScreenSingleton::getInstance().SetColor(CC_Blue);
}

std::string Plane::ClassID() const {
  return "Plane";
}

void ColorPlane::DrawBody() const
{
    ScreenSingleton::getInstance().SetColor(CC_Green);
    ScreenSingleton::getInstance().GotoXY(x, y);
    std::cout << "=========>";
}

void ColorPlane::DrawWings() const
{
    ScreenSingleton::getInstance().SetColor(CC_Cyan);
    ScreenSingleton::getInstance().GotoXY(x + 3, y - 1);
    std::cout << "\\\\\\\\";
    ScreenSingleton::getInstance().GotoXY(x + 3, y + 1);
    std::cout << "////";
}

void ColorPlane::DrawTail() const
{
    ScreenSingleton::getInstance().SetColor(CC_Red);
    ScreenSingleton::getInstance().GotoXY(x - 2, y - 1);
     std::cout << "===";
}

void BigPlane::DrawBody() const
{
    ScreenSingleton::getInstance().SetColor(CC_Blue);
    ScreenSingleton::getInstance().GotoXY(x, y);
    std::cout << "======|||===>";
}

void BigPlane::DrawWings() const
{
    ScreenSingleton::getInstance().SetColor(CC_Blue);
    ScreenSingleton::getInstance().GotoXY(x + 3, y - 1);
    std::cout << "\\\\\\\\\\";
    ScreenSingleton::getInstance().GotoXY(x + 3, y + 1);
    std::cout << "//////";
    ScreenSingleton::getInstance().GotoXY(x + 3, y + 2);
    std::cout << "//////";
}

void BigPlane::DrawTail() const
{
    ScreenSingleton::getInstance().SetColor(CC_Blue);
    ScreenSingleton::getInstance().GotoXY(x - 2, y - 1);
    std::cout << "==|||=";
}
