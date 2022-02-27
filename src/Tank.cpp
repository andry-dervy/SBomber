
#include <iostream>

#include "Tank.h"
#include "MyTools.h"
#include "ScreenSingleton.h"

using namespace std;
using namespace MyTools;

bool Tank::isInside(double x1, double x2) const
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

void Tank::Draw() const
{
	ScreenSingleton::getInstance().SetColor(CC_Cyan);
	ScreenSingleton::getInstance().GotoXY(x, y - 3);
	cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x-2, y - 2);
	cout << "#######   #";
	ScreenSingleton::getInstance().GotoXY(x, y - 1);
	cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x,y);
	cout << " ###########";
}

std::string Tank::ClassID() const {
  return "Tank";
}

void Tank::sendMessage()
{
    uint64_t time_now = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    if((time_now - time_pnt) > 5000 && mediator != nullptr)
    {
        time_pnt = time_now;

        std::unique_ptr<TankMessage> mes = std::make_unique<TankMessage>();
        mes->text = "Didn't hit.";
        mes->x = GetX();
        mes->y = GetY() - 5;

        mediator->sendMessage(std::move(mes));
    }
}

void Tank::addMediator(std::shared_ptr<Mediator>& amediator)
{
    mediator = amediator;
}
