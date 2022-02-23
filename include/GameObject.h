#pragma once

#include <cstdint>
#include <string>

class Observer
{
public:
  virtual ~Observer() = default;
  virtual bool HandleInsideCheck(double x1, double x2) = 0;
};

class GameObject: public Observer {
public:
  GameObject() : x(0.0), y(0.0), width(0) {}
  virtual ~GameObject() = default;

  virtual void Draw() const = 0;
  virtual std::string ClassID() const = 0;

  inline void SetPos(double nx, double ny) {
    x = nx;
    y = ny;
  }

  inline double GetY() const { return y; }
  inline double GetX() const { return x; }

  inline void SetWidth(uint16_t widthN) { width = widthN; }
  inline uint16_t GetWidth() const { return width; }

  bool HandleInsideCheck(double x1, double x2) override {
    return false;
  }

protected:
  double x, y;
  uint16_t width;
};
