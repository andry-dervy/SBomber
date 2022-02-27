#pragma once

#include <memory>
#include <vector>

class Tank;
class LevelGUI;
struct TankMessage;

class Mediator
{
public:
    void addTank(std::shared_ptr<Tank>& tank)
        {vtanks.push_back(tank);}
    void addLevelGUI(std::shared_ptr<LevelGUI>& alevelgui)
        {levelgui = alevelgui;}
    void sendMessage(std::unique_ptr<TankMessage>&& mes);

private:
    std::vector<std::shared_ptr<Tank>> vtanks;
    std::shared_ptr<LevelGUI> levelgui;
};
