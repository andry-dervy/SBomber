#pragma once

#include <list>
#include <memory>

#include "LevelGUI.h"
#include "Plane.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "enums/CraterSize.h"

class Command
{
protected:

public:
  virtual ~Command() {}
  virtual void Execute() = 0;
};

class CommandDeleteDynamicObj: public Command
{
public:
    CommandDeleteDynamicObj(std::shared_ptr<DynamicObject> pObj,
                            std::vector<std::shared_ptr<DynamicObject>>& avecDynamicObj)
      : obj{pObj},vecDynamicObj{avecDynamicObj}{}
    void Execute() override;

private:
    std::shared_ptr<DynamicObject> obj;
    std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj;
};

class CommandDeleteStaticObj: public Command
{
public:
    CommandDeleteStaticObj(std::shared_ptr<GameObject> pObj,
                            std::vector<std::shared_ptr<GameObject>>& avecStaticObj)
      : obj{pObj},vecStaticObj{avecStaticObj}{}
    void Execute() override;

private:
    std::shared_ptr<GameObject> obj;
    std::vector<std::shared_ptr<GameObject>>& vecStaticObj;
};

class CommandDropBomb: public Command
{
public:
    CommandDropBomb(std::shared_ptr<const Plane> pPlane,
                    std::vector<std::shared_ptr<DynamicObject>>& avecDynamicObj,
                    uint16_t& aBombsNumber,
                    int16_t& aScore,
                    double aSpeedBomb = 1,
                    uint16_t aWidthCrater = SMALL_CRATER_SIZE)
      : plane{pPlane}, vecDynamicObj{avecDynamicObj}, bombsNumber{aBombsNumber},
        score{aScore}, speedBomb{aSpeedBomb}, widthCrater{aWidthCrater} {}
    void Execute() override;

protected:
    std::shared_ptr<const Plane> plane;
    std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj;
    uint16_t& bombsNumber;
    int16_t& score;
    double speedBomb;
    uint16_t widthCrater;
};

class CommandDropBombDecorator: public CommandDropBomb
{
public:
    CommandDropBombDecorator(std::shared_ptr<const Plane> pPlane,
                    std::vector<std::shared_ptr<DynamicObject>>& avecDynamicObj,
                    uint16_t& aBombsNumber,
                    int16_t& aScore,
                    double aSpeedBomb = 1,
                    uint16_t aWidthCrater = SMALL_CRATER_SIZE)
      : CommandDropBomb{pPlane,avecDynamicObj,aBombsNumber,
        aScore, aSpeedBomb, aWidthCrater} {}
    void Execute() override;
};


class MacroCommand
{
private:
  std::list<std::unique_ptr<Command>> fifoCommands;

public:
  MacroCommand() = default;
  ~MacroCommand() = default;

  void addCommand(std::unique_ptr<Command>&& command)
  {
    fifoCommands.push_back(std::move(command));
  }

  void Run()
  {
    for(auto& command: fifoCommands)
    {
      command->Execute();
    }
    fifoCommands.clear();
  }
};

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
    void RunCommands();

private:
    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void  CheckDestoyableObjects(std::shared_ptr<Bomb> pBomb);

    std::shared_ptr<Ground> FindGround() const;
    std::shared_ptr<Plane> FindPlane() const;
    std::shared_ptr<LevelGUI> FindLevelGUI() const;
    std::vector<std::shared_ptr<DestroyableGroundObject>> FindDestoyableGroundObjects() const;
    std::vector<std::shared_ptr<Bomb>> FindAllBombs() const;

private:
    MacroCommand macroCommand;

    std::vector<std::shared_ptr<DynamicObject>> vecDynamicObj;
    std::vector<std::shared_ptr<GameObject>> vecStaticObj;

    bool exitFlag;

    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;
};

