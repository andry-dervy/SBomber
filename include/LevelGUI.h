#pragma once

#include <stdint.h>
#include <memory>
#include <queue>

#include "GameObject.h"
#include <Tank.h>
#include <Mediator.h>

class LevelGUI : public GameObject {
public:

    LevelGUI() : bombsNumber(0), score(0), passedTime(0), fps(0), height(0) {
        passedTimeTankMes = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    void  SetParam(uint64_t passedTimeNew, uint64_t fpsNew, uint16_t bombsNumberNew, int16_t scoreNew);
    
    void  SetHeight(uint16_t heightN) { height = heightN; };
    
    inline uint16_t GetFinishX() const { return finishX; }
    inline void SetFinishX(uint16_t finishXN) { finishX = finishXN; }

    void Draw() const override;
    std::string ClassID() const override;
    void addMediator(std::shared_ptr<Mediator>& amediator) {
        mediator = amediator;
    }
    void recieveMesssage(std::unique_ptr<TankMessage>&& tankmes) {
        qmessages.push(std::move(tankmes));
    }
    void newTankMessage();

private:
    std::shared_ptr<Mediator> mediator;
    std::queue<std::unique_ptr<TankMessage>> qmessages;
    std::unique_ptr<const TankMessage> tankmes;

    uint16_t height;
    uint16_t finishX = 109;

    uint64_t passedTimeTankMes;
    uint64_t passedTime, fps;
    uint16_t bombsNumber;
    int16_t score;
};



