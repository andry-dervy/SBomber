#pragma once

#include <memory>
#include "DynamicObject.h"
#include "Visitor.h"

class Bomb : public DynamicObject
{
public:
  static const uint16_t BombCost = 10;

public:
  void Draw() const override;
  std::string ClassID() const override;
  void Accept(const Visitor& v) {v.log(*this);}

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
  double GetSpeed() const {return bomb->GetSpeed();}
  void Accept(const Visitor& v) {v.log(*bomb);}

public:
  void Draw() const override;
  std::string ClassID() const override;
  void Move(uint16_t time) override;
};
