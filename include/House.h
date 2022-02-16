#pragma once

#include "DestroyableGroundObject.h"
#include <cstdint>
#include <cstring>
#include <memory>

class House;

enum TypeHouse
{
  HouseWithPipe,
  HouseWithoutPipe,
};

class HouseBuilder
{
protected:
  std::shared_ptr<House> house;
public:
  HouseBuilder():house(std::make_shared<House>()){}
  virtual ~HouseBuilder() = default;
  std::shared_ptr<House> getHouse();
  virtual void ConstructorFloor() = 0;
  virtual void ConstructorWallLeft() = 0;
  virtual void ConstructorWallRight() = 0;
  virtual void ConstructorCeiling() = 0;
  virtual void ConstructorRoof() = 0;
  virtual void ConstructorPipe() = 0;
};

class HouseBuilderA: public HouseBuilder
{
public:
  HouseBuilderA() = default;
  void ConstructorFloor() override;
  void ConstructorWallLeft() override;
  void ConstructorWallRight() override;
  void ConstructorCeiling() override;
  void ConstructorRoof() override;
  void ConstructorPipe() override;
};

class HouseDirector
{
public:
  std::shared_ptr<House> constructorHouseWithPipe(HouseBuilderA& builder);
  std::shared_ptr<House> constructorHouseWithoutPipe(HouseBuilderA& builder);
};

class House : public DestroyableGroundObject {
public:
  House();
  bool isInside(double x1, double x2) const override;

  inline uint16_t GetScore() const override {
    return score;
  }

  void Draw() const override;
  std::string ClassID() const override;

private:
  const uint16_t score = 40;

  char look[7][14];

  friend HouseBuilderA;
};
