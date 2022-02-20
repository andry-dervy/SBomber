#include "Bomb.h"
#include <iostream>
#include "ScreenSingleton.h"

void Bomb::Draw() const {
  //ScreenSingleton::getInstance().SetColor(CC_LightMagenta);
  ScreenSingleton::getInstance().GotoXY(x, y);
  std::cout << "*";
}

std::string Bomb::ClassID() const {
  return "Bomb";
}

void BombDecorator::Draw() const {
  ScreenSingleton::getInstance().GotoXY(x, y-1);
  std::cout << "|";
  bomb->Draw();
}

std::string BombDecorator::ClassID() const {
  return "BombDecorator";
}

void BombDecorator::Move(uint16_t time)
{
  bomb->Move(1.5 * time);
  SetPos(bomb->GetX(),bomb->GetY());
  auto dir = bomb->GetDirection();
  SetDirection(dir.first,dir.second);
};
