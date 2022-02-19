#pragma once

#include <iostream>
#include <memory>

#include "ScreenSingleton.h"
#include "MyTools.h"
#include "DestroyableGroundObject.h"


class Tower
{
public:
  Tower() : x(0.0), y(0.0), width(0) {}
  virtual ~Tower() = default;

  void Paint() const;

  bool isInRange(double x1, double x2) const;

  inline void SetPos(double nx, double ny) {
    x = nx;
    y = ny;
  }

  inline double GetY() const { return y; }
  inline double GetX() const { return x; }

  inline void SetWidth(uint16_t widthN) { width = widthN; }
  inline uint16_t GetWidth() const { return width; }

protected:
  double x, y;
  uint16_t width;
};

class TowerAdapter: public DestroyableGroundObject
{
public:
  TowerAdapter(std::unique_ptr<Tower>&& aTower)
    :tower(std::move(aTower)){}
  ~TowerAdapter(){}

  bool  isInside(double x1, double x2) const override;

  inline uint16_t GetScore() const override { return score; }

  void Draw() const override;
  std::string ClassID() const override;

  void SetWidth(uint16_t widthN);
  void SetPos(double nx, double ny);
private:
  std::unique_ptr<Tower> tower;

  const uint16_t score = 25;
};

