#pragma once

#include "DynamicObject.h"
#include "Visitor.h"

class Plane : public DynamicObject {
public:
    void Draw() const override;
    std::string ClassID() const override;

public:
    inline void ChangePlaneY(double dy) { yDirection += dy; }

public:
    void Accept(const Visitor& v) {v.log(*this);}
};

