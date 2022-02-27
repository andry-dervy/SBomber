#pragma once

#include "DynamicObject.h"

class Plane : public DynamicObject {
public:

    void Draw() const override;

    inline void ChangePlaneY(double dy) { yDirection += dy; }
    std::string ClassID() const override;


private:
    virtual void DrawBody() const = 0;
    virtual void DrawWings() const = 0;
    virtual void DrawTail() const = 0;
};

class ColorPlane: public Plane
{
private:
    void DrawBody() const override;
    void DrawWings() const override;
    void DrawTail() const override;
};

class BigPlane: public Plane
{
private:
    void DrawBody() const override;
    void DrawWings() const override;
    void DrawTail() const override;
};
