#include "House.h"
#include <iostream>
#include "MyTools.h"
#include "ScreenSingleton.h"

House::House()
{
  for(size_t i = 0; i < 7; ++i)
  {
    std::memset(look[i],' ',13);
    look[i][13] = 0;
  }
}

bool House::isInside(double x1, double x2) const
{
	const double XBeg = x + 2;
	const double XEnd = x + width - 1;

	if (x1 < XBeg && x2 > XEnd)
	{
		return true;
	}

	if (x1 > XBeg && x1 < XEnd)
	{
		return true;
	}

	if (x2 > XBeg && x2 < XEnd)
	{
		return true;
	}

	return false;
}

void House::Draw() const
{
  ScreenSingleton::getInstance().SetColor(CC_Yellow);
  ScreenSingleton::getInstance().GotoXY(x, y - 6);
  std::cout << look[6];
  ScreenSingleton::getInstance().GotoXY(x, y - 5);
  std::cout << look[5];
  ScreenSingleton::getInstance().GotoXY(x, y - 4);
  std::cout << look[4];
  ScreenSingleton::getInstance().GotoXY(x, y - 3);
  std::cout << look[3];
  ScreenSingleton::getInstance().GotoXY(x, y - 2);
  std::cout << look[2];
  ScreenSingleton::getInstance().GotoXY(x, y - 1);
  std::cout << look[1];
  ScreenSingleton::getInstance().GotoXY(x, y);
  std::cout << look[0];
}

std::string House::ClassID() const {
  return "House";
}

std::shared_ptr<House> HouseBuilder::getHouse()
{
  return house;
}

void HouseBuilderA::ConstructorFloor()
{
  std::memset(&(house->look[0][4]),'#',4);
}
void HouseBuilderA::ConstructorWallLeft()
{
  std::memset(&(house->look[0][2]),'#',2);
  std::memset(&(house->look[1][2]),'#',2);
  std::memset(&(house->look[2][2]),'#',2);
  std::memset(&(house->look[3][2]),'#',2);
}
void HouseBuilderA::ConstructorWallRight()
{
  std::memset(&(house->look[0][8]),'#',2);
  std::memset(&(house->look[1][8]),'#',2);
  std::memset(&(house->look[2][8]),'#',2);
  std::memset(&(house->look[3][8]),'#',2);
}
void HouseBuilderA::ConstructorCeiling()
{
  std::memset(&(house->look[3][4]),'#',4);
}
void HouseBuilderA::ConstructorRoof()
{
  std::memset(&(house->look[3][0]),'#',2);
  std::memset(&(house->look[4][1]),'#',2);
  std::memset(&(house->look[5][2]),'#',2);
  std::memset(&(house->look[6][3]),'#',6);
  std::memset(&(house->look[5][8]),'#',2);
  std::memset(&(house->look[4][9]),'#',2);
  std::memset(&(house->look[3][10]),'#',2);
}
void HouseBuilderA::ConstructorPipe()
{
  std::memset(&(house->look[5][10]),'#',1);
  std::memset(&(house->look[6][10]),'#',1);
}

std::shared_ptr<House> HouseDirector::constructorHouseWithPipe(HouseBuilderA& builder)
{
  builder.ConstructorFloor();
  builder.ConstructorWallLeft();
  builder.ConstructorWallRight();
  builder.ConstructorCeiling();
  builder.ConstructorRoof();
  builder.ConstructorPipe();
  return builder.getHouse();
}

std::shared_ptr<House> HouseDirector::constructorHouseWithoutPipe(HouseBuilderA& builder)
{
  builder.ConstructorFloor();
  builder.ConstructorWallLeft();
  builder.ConstructorWallRight();
  builder.ConstructorCeiling();
  builder.ConstructorRoof();
  return builder.getHouse();
}
