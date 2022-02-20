#include "Visitor.h"
#include "MyTools.h"

#include "Bomb.h"
#include "Plane.h"

#include <iostream>

void LogVisitor::log(const Bomb& bomb) const
{
  MyTools::LoggerSingleton::getInstance().WriteToLog("Bomb: x = ",  bomb.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Bomb: y = ",  bomb.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Bomb: speed = ",  bomb.GetSpeed() );
  auto direction = bomb.GetDirection();
  MyTools::LoggerSingleton::getInstance().WriteToLog("Bomb: xDirection = ",  direction.first );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Bomb: yDirection = ",  direction.second );
}

void LogVisitor::log(const BombDecorator& bomb) const
{
  MyTools::LoggerSingleton::getInstance().WriteToLog("BombDecorator: x = ",  bomb.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("BombDecorator: y = ",  bomb.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("BombDecorator: speed = ",  bomb.GetSpeed() );
  auto direction = bomb.GetDirection();
  MyTools::LoggerSingleton::getInstance().WriteToLog("BombDecorator: xDirection = ",  direction.first );
  MyTools::LoggerSingleton::getInstance().WriteToLog("BombDecorator: yDirection = ",  direction.second );
}

void LogVisitor::log(const Plane& plane) const
{
  MyTools::LoggerSingleton::getInstance().WriteToLog("Plane: x = ",  plane.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Plane: y = ",  plane.GetX() );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Plane: speed = ",  plane.GetSpeed() );
  auto direction = plane.GetDirection();
  MyTools::LoggerSingleton::getInstance().WriteToLog("Plane: xDirection = ",  direction.first );
  MyTools::LoggerSingleton::getInstance().WriteToLog("Plane: yDirection = ",  direction.second );
}
