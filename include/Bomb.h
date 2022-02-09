#pragma once

#include <memory>
#include "DynamicObject.h"

class Bomb : public DynamicObject
{
public:

  static const uint16_t BombCost = 10;

	void Draw() const override;
  std::string ClassID() const override;

private:

};

class BombDecorator : public DynamicObject
{
private:
  std::unique_ptr<Bomb> bomb;

public:
  BombDecorator(std::unique_ptr<Bomb>&& aBomb)
    :bomb(std::move(aBomb))
  {
    SetWidth(bomb->GetWidth());
  }
  ~BombDecorator() = default;
public:
  void Draw() const override;
  std::string ClassID() const override;
  void Move(uint16_t time) override;
};
