#pragma once

#include "DynamicObject.h"

class Bomb : public DynamicObject
{
public:

  static const uint16_t BombCost = 10;

	void Draw() const override;
  std::string ClassID() const override;

private:

};

