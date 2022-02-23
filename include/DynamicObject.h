#pragma once

#include <stdint.h>
#include <utility>
#include <algorithm>
#include <memory>

#include "GameObject.h"
#include "DestroyableGroundObject.h"

class DynamicObject : public GameObject 
{
public:

    DynamicObject() : speed(0.0), xDirection(0.0), yDirection(0) { }

    inline void SetSpeed(double sp) { speed = sp; }
    double GetSpeed() const {return speed;}
    inline void SetDirection(double dx, double dy) { xDirection = dx; yDirection = dy; }
    std::pair<double, double> GetDirection() const {return std::make_pair(xDirection,yDirection);}

    virtual void Move(uint16_t time);
    virtual std::vector<std::shared_ptr<DestroyableGroundObject>> CheckDestroyableObjects(int16_t& score);

    void AddObserver(std::shared_ptr<DestroyableGroundObject> observer);
    void AddObservers(std::vector<std::shared_ptr<DestroyableGroundObject>>& observers);
    void DelObserver(const std::shared_ptr<DestroyableGroundObject>& observer);

protected:
    std::vector<std::shared_ptr<DestroyableGroundObject>> observers_;

protected:
    double speed;
    double xDirection, yDirection;

};
