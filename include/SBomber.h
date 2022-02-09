#pragma once

#include <vector>
#include <memory>

#include "LevelGUI.h"
#include "Plane.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"

class SBomber
{
public:

    SBomber();
    ~SBomber();
    
    inline bool GetExitFlag() const { return exitFlag; }

    void ProcessKBHit(int amountInputtedCodes);
    void TimeStart();
    void TimeFinish();

    void DrawFrame();
    void MoveObjects();
    void CheckObjects();

private:

    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void  CheckDestoyableObjects(std::shared_ptr<Bomb> pBomb);

    void  DeleteDynamicObj(std::shared_ptr<DynamicObject> pObj);
    void  DeleteStaticObj(std::shared_ptr<GameObject> pObj);

    std::shared_ptr<Ground> FindGround() const;
    std::shared_ptr<Plane> FindPlane() const;
    std::shared_ptr<LevelGUI> FindLevelGUI() const;
    std::vector<std::shared_ptr<DestroyableGroundObject>> FindDestoyableGroundObjects() const;
    std::vector<std::shared_ptr<Bomb>> FindAllBombs() const;

    void DropBomb();

    std::vector<std::shared_ptr<DynamicObject>> vecDynamicObj;
    std::vector<std::shared_ptr<GameObject>> vecStaticObj;
    
    bool exitFlag;

    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;
};
