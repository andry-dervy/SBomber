#pragma once

#include <memory>
#include "DynamicObject.h"

class Prototype
{
public:
    virtual ~Prototype() = default;
    virtual std::shared_ptr<DynamicObject> clone() = 0;
};

class Bomb : public DynamicObject, Prototype
{
public:

    static const uint16_t BombCost = 10;

	void Draw() const override;
    std::string ClassID() const override;
    std::shared_ptr<DynamicObject> clone() override;
private:

};

class BombDecorator : public DynamicObject, Prototype
{
private:
  std::shared_ptr<Bomb> bomb;

public:
  BombDecorator(std::shared_ptr<Bomb>&& aBomb)
    :bomb(std::move(aBomb))
  {
    SetWidth(bomb->GetWidth());
  }
  ~BombDecorator() = default;
public:
  void Draw() const override;
  std::string ClassID() const override;
  void Move(uint16_t time) override;
  std::shared_ptr<DynamicObject> clone() override;
};
