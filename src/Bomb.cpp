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

std::shared_ptr<DynamicObject> Bomb::clone()
{
    std::shared_ptr<Bomb> ptr = std::make_shared<Bomb>();

    ptr->SetSpeed(speed);
    ptr->SetDirection(xDirection,yDirection);
    ptr->SetWidth(width);
    ptr->SetPos(x,y);

    return ptr;
}

void BombDecorator::Draw() const {
  ScreenSingleton::getInstance().GotoXY(x, y-1);
  std::cout << "|";
  bomb->Draw();
}

std::string BombDecorator::ClassID() const {
  return bomb->ClassID();
}

void BombDecorator::Move(uint16_t time)
{
  bomb->Move(1.5 * time);
  SetPos(bomb->GetX(),bomb->GetY());
};

std::shared_ptr<DynamicObject> BombDecorator::clone()
{
    std::shared_ptr<BombDecorator> ptr =
            std::make_shared<BombDecorator>(
                std::static_pointer_cast<Bomb>(bomb->clone()));

    ptr->SetSpeed(speed);
    ptr->SetDirection(xDirection,yDirection);
    ptr->SetWidth(width);
    ptr->SetPos(x,y);

    return ptr;
}
