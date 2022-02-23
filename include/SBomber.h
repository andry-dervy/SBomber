#pragma once

#include <chrono>
#include <thread>
#include <list>
#include <memory>
#include <iostream>

#include "MyTools.h"
#include "ScreenSingleton.h"
#include "LevelGUI.h"
#include "House.h"
#include "Plane.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "Tower.h"
#include "Visitor.h"
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
                    std::vector<std::shared_ptr<DestroyableGroundObject>> avecDestrGroundObj,
                    uint16_t& aBombsNumber,
                    int16_t& aScore,
                    double aSpeedBomb = 1,
                    uint16_t aWidthCrater = SMALL_CRATER_SIZE)
      : plane{pPlane}, vecDynamicObj{avecDynamicObj},
        vecDestrGroundObj{avecDestrGroundObj}, bombsNumber{aBombsNumber},
        score{aScore}, speedBomb{aSpeedBomb}, widthCrater{aWidthCrater} {}
    void Execute() override;

protected:
    std::shared_ptr<const Plane> plane;
    std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj;
    std::vector<std::shared_ptr<DestroyableGroundObject>> vecDestrGroundObj;
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
                    std::vector<std::shared_ptr<DestroyableGroundObject>> avecDestrGroundObj,
                    uint16_t& aBombsNumber,
                    int16_t& aScore,
                    double aSpeedBomb = 1,
                    uint16_t aWidthCrater = SMALL_CRATER_SIZE)
      : CommandDropBomb{pPlane,avecDynamicObj,avecDestrGroundObj,aBombsNumber,
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

  void addCommands(std::unique_ptr<MacroCommand> command)
  {
    for(auto& com: command->fifoCommands)
      fifoCommands.push_back(std::move(com));
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

//CheckBridge
class ICheckImpl
{
public:
  virtual ~ICheckImpl() {};

  virtual void CheckPlaneAndLevelGUI(
      std::shared_ptr<Plane> plane,
      std::shared_ptr<LevelGUI> levelGui,
      bool& exitFlag) = 0;
  virtual std::unique_ptr<MacroCommand> CheckBombsAndGround(
      std::vector<std::shared_ptr<DynamicObject>> vecBombs,
      const std::shared_ptr<Ground>& pGround,
      std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj,
      std::vector<std::shared_ptr<GameObject>>& vecStaticObj,
      int16_t& score) = 0;
};

class ConcreteCheckImpl : public ICheckImpl
{
public:
  virtual ~ConcreteCheckImpl(){}

  void CheckPlaneAndLevelGUI(
      std::shared_ptr<Plane> plane,
      std::shared_ptr<LevelGUI> levelGui,
      bool& exitFlag) override;
  std::unique_ptr<MacroCommand> CheckBombsAndGround(
      std::vector<std::shared_ptr<DynamicObject>> vecBombs,
      const std::shared_ptr<Ground>& pGround,
      std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj,
      std::vector<std::shared_ptr<GameObject>>& vecStaticObj,
      int16_t& score) override;
};

class SBomber
{
public:

    SBomber(std::unique_ptr<ICheckImpl> acheckImpl);
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
    uint32_t getKeyCode(int amountInputtedCodes);
    TypeHouse EnterTypeHouse();

    std::shared_ptr<Ground> FindGround() const;
    std::shared_ptr<Plane> FindPlane() const;
    std::shared_ptr<LevelGUI> FindLevelGUI() const;
    std::vector<std::shared_ptr<DestroyableGroundObject>> FindDestoyableGroundObjects() const;
    std::vector<std::shared_ptr<DynamicObject>> FindAllBombs();

private:
    std::unique_ptr<ICheckImpl> checkImpl;
    MacroCommand macroCommand;
    LogVisitor logDynObjects;


    std::vector<std::shared_ptr<DynamicObject>> vecDynamicObj;
    std::vector<std::shared_ptr<GameObject>> vecStaticObj;

    bool exitFlag;

    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;

public:
    class BombIterator
    {
    private:
      size_t indx;
      const std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj;

    public:
      BombIterator(const std::vector<std::shared_ptr<DynamicObject>> &avecDynamicObj)
        :indx(0),vecDynamicObj(avecDynamicObj) {
        for(;indx < vecDynamicObj.size(); ++indx)
        {
          if(vecDynamicObj[indx]->ClassID() == "Bomb" ||
             vecDynamicObj[indx]->ClassID() == "BombDecorator")
          {
            return;
          }
        }
      }

      void reset(){indx = vecDynamicObj.size();}

      BombIterator& operator++() {
        for(++indx; indx < vecDynamicObj.size(); ++indx)
        {
          if(vecDynamicObj[indx]->ClassID() == "Bomb"||
             vecDynamicObj[indx]->ClassID() == "BombDecorator")
          {
            return *this;
          }
        }
        return *this;
      }

      bool operator==(BombIterator& it) {
        if(indx == it.indx) return true;
        return false;
      }

      bool operator!=(BombIterator& it) {
        return !(*this == it);
      }

      std::shared_ptr<DynamicObject>& operator*() {
        return const_cast<std::shared_ptr<DynamicObject>&>(vecDynamicObj.at(indx));
      }

      std::shared_ptr<DynamicObject>& operator->() {
        return const_cast<std::shared_ptr<DynamicObject>&>(vecDynamicObj.at(indx));
      }

    };

    BombIterator begin(const std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj) const {
      return BombIterator(vecDynamicObj);
    }

    BombIterator end(const std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj) const {
      BombIterator it{vecDynamicObj};
      it.reset();
      return it;
    }

};
