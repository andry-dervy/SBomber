#pragma once

#include <stdint.h>
#include <utility>

#include "GameObject.h"

class DynamicObject : public GameObject 
{
public:

    DynamicObject() : speed(0.0), xDirection(0.0), yDirection(0) { }

    inline void SetSpeed(double sp) { speed = sp; }
    double GetSpeed() const {return speed;}
    inline void SetDirection(double dx, double dy) { xDirection = dx; yDirection = dy; }
    std::pair<double, double> GetDirection() const {return std::make_pair(xDirection,yDirection);}

    virtual void Move(uint16_t time);

protected:

    double speed;
    double xDirection, yDirection;

};
