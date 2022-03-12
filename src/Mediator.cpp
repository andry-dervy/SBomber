
#include <Mediator.h>
#include <Tank.h>
#include <LevelGUI.h>

void Mediator::sendMessage(std::unique_ptr<TankMessage>&& mes) {
    if(levelgui != nullptr)
    {
        levelgui->recieveMesssage(std::move(mes));
    }
}
