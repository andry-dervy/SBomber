
#include "DynamicObject.h"

void DynamicObject::Move(uint16_t time)
{
  x += xDirection * speed * time * 0.001;
  y += yDirection * speed * time * 0.001;
}
