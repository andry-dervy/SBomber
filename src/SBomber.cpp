
#include "SBomber.h"

#if defined(_WIN32) || defined(WIN32)
#include <conio.h>
#include <windows.h>
#endif

#include <cassert>
#include <chrono>
#include <random>

int getRandomNum(int min, int max)
{
  const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::mt19937_64 generator(seed);
  std::uniform_int_distribution<int> dis(min, max);
  return dis(generator);
}

SBomber::SBomber(std::unique_ptr<ICheckImpl> acheckImpl)
  : checkImpl(std::move(acheckImpl)), exitFlag(false), startTime(0), finishTime(0), deltaTime(0), passedTime(0),
    fps(0), bombsNumber(10), score(0) {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

  std::unique_ptr<Plane> pPlane;
  if(getRandomNum(0,100)%2)
  {
      pPlane = std::make_unique<ColorPlane>();
  }
  else
  {
      pPlane = std::make_unique<BigPlane>();
  }
  pPlane->SetDirection(1, 0.1);
  pPlane->SetSpeed(4);
  pPlane->SetPos(5, 10);
  vecDynamicObj.push_back(std::move(pPlane));

  std::shared_ptr<Mediator> mediator = std::make_shared<Mediator>();
  std::shared_ptr<LevelGUI> pGUI {new LevelGUI};
  pGUI->SetParam(passedTime, fps, bombsNumber, score);
  const uint16_t maxX = ScreenSingleton::getInstance().GetMaxX();
  const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
  const uint16_t offset = 3;
  const uint16_t width = maxX - 7;
  pGUI->SetPos(offset, offset);
  pGUI->SetWidth(width);
  pGUI->SetHeight(maxY - 4);
  pGUI->SetFinishX(offset + width - 4);
  mediator->addLevelGUI(pGUI);
  pGUI->addMediator(mediator);
  vecStaticObj.push_back(std::move(pGUI));

  std::unique_ptr<Ground> pGr {new Ground};
  const uint16_t groundY = maxY - 5;
  pGr->SetPos(offset + 1, groundY);
  pGr->SetWidth(width - 2);
  vecStaticObj.push_back(std::move(pGr));

  std::shared_ptr<Tank> pTank{new Tank};
  pTank->SetWidth(13);
  pTank->SetPos(15, groundY - 1);
  mediator->addTank(pTank);
  pTank->addMediator(mediator);
  vecStaticObj.push_back(std::move(pTank));

  std::unique_ptr<Tower> pTower{new Tower};
  std::unique_ptr<TowerAdapter> pTowerAdapter{new TowerAdapter(std::move(pTower))};
  pTowerAdapter->SetWidth(13);
  pTowerAdapter->SetPos(35, groundY - 1);
  vecStaticObj.push_back(std::move(pTowerAdapter));

  HouseBuilderA builder;
  HouseDirector houseDir;
  std::shared_ptr<House> pHouse;
  switch (EnterTypeHouse()) {
  case TypeHouse::HouseWithPipe:
    pHouse = houseDir.constructorHouseWithPipe(builder);
    break;
  case TypeHouse::HouseWithoutPipe:
    pHouse = houseDir.constructorHouseWithoutPipe(builder);
    break;
  }
  pHouse->SetWidth(13);
  pHouse->SetPos(55, groundY - 1);
  vecStaticObj.push_back(std::move(pHouse));

  /*
  Bomb* pBomb = new Bomb;
  pBomb->SetDirection(0.3, 1);
  pBomb->SetSpeed(2);
  pBomb->SetPos(51, 5);
  pBomb->SetSize(SMALL_CRATER_SIZE);
  vecDynamicObj.push_back(pBomb);
  */
}

SBomber::~SBomber() {
}

void SBomber::MoveObjects() {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

  for (size_t i = 0; i < vecDynamicObj.size(); i++) {
    if (vecDynamicObj[i] != nullptr) {
      vecDynamicObj[i]->Move(deltaTime);
    }
  }
};

void SBomber::CheckObjects() {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

  checkImpl->CheckPlaneAndLevelGUI(
          FindPlane(),
          FindLevelGUI(),
          exitFlag);
  macroCommand.addCommands(
        checkImpl->CheckBombsAndGround(
          FindAllBombs(),
          FindGround(),
          vecDynamicObj,
          FindDestoyableGroundObjects(),
          vecStaticObj,
          score));
};

std::vector<std::shared_ptr<DestroyableGroundObject>> SBomber::FindDestoyableGroundObjects() const {
  std::vector<std::shared_ptr<DestroyableGroundObject>> vec;
  for (size_t i = 0; i < vecStaticObj.size(); i++) {
    if (vecStaticObj[i]->ClassID() == "Tank" ||
        vecStaticObj[i]->ClassID() == "Tower" ||
        vecStaticObj[i]->ClassID() == "House") {
      vec.push_back(std::static_pointer_cast<DestroyableGroundObject>(vecStaticObj[i]));
    }
  }

  return vec;
}

std::shared_ptr<Ground> SBomber::FindGround() const {
  for (size_t i = 0; i < vecStaticObj.size(); i++) {
    if (vecStaticObj[i]->ClassID() == "Ground") {
      return std::static_pointer_cast<Ground>(vecStaticObj[i]);
    }
  }

  return nullptr;
}

std::vector<std::shared_ptr<Bomb>> SBomber::FindAllBombs() {
  std::vector<std::shared_ptr<Bomb>> vecBombs;

  BombIterator it = begin(vecDynamicObj);
  BombIterator it_end = end(vecDynamicObj);
  for (; it != it_end; ++it) {
      vecBombs.push_back(std::static_pointer_cast<Bomb>(*it));
  }

  return vecBombs;
}

std::shared_ptr<Plane> SBomber::FindPlane() const {
  for (size_t i = 0; i < vecDynamicObj.size(); i++) {
    if (vecDynamicObj[i]->ClassID() == "Plane") {
      return std::static_pointer_cast<Plane>(vecDynamicObj[i]);
    }
  }

  return nullptr;
}

std::shared_ptr<LevelGUI> SBomber::FindLevelGUI() const {
  for (size_t i = 0; i < vecStaticObj.size(); i++) {
    if (vecStaticObj[i]->ClassID() == "LevelGUI") {
      return std::static_pointer_cast<LevelGUI>(vecStaticObj[i]);
    }
  }

  return nullptr;
}

TypeHouse SBomber::EnterTypeHouse() {
  std::cout << "Enter '1' to build House with a pipe." << std::endl;
  std::cout << "Enter '2' to build House without a pipe." << std::endl;
  std::cout << " < ";

  uint32_t c;
  std::cin >> c;

  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked. key = ", static_cast<int>(c));

  switch (c)
  {
    case 1: // HouseWithPipe
      return TypeHouse::HouseWithPipe;

    case 2: // HouseWithoutPipe
      return TypeHouse::HouseWithoutPipe;

    default:
      return TypeHouse::HouseWithPipe;
  }
}

void SBomber::ProcessKBHit(int amountInputtedCodes) {
  assert(amountInputtedCodes >= 0 && amountInputtedCodes < 4);

  if(amountInputtedCodes == 0) return;

#if defined(_WIN32) || defined(WIN32)
  int c = _getch();

  if (c == 224)
  {
      c = _getch();
  }
#else
  uint32_t c = 0;
  uint8_t* p = (uint8_t*)(&c);
  for (int i = 0; i < amountInputtedCodes; ++i)
  {
      *p++ = static_cast<uint8_t>(getchar());
  }
#endif

  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked. key = ", static_cast<int>(c));

  switch (c)
  {

    case 27: // esc
      exitFlag = true;
      break;

#if defined(_WIN32) || defined(WIN32)
    case 72: // up
#else
    case 0x415b1b: // up
#endif
      FindPlane()->ChangePlaneY(-0.25);
      break;

#if defined(_WIN32) || defined(WIN32)
    case 80: // down
#else
    case 0x425b1b: // down
#endif
      FindPlane()->ChangePlaneY(0.25);
      break;

    case 'b': // DropBomb
      {
        std::unique_ptr<CommandDropBomb> pComDropBomb =
            std::make_unique<CommandDropBomb>(
              FindPlane(),vecDynamicObj,bombsNumber,score
              );
        macroCommand.addCommand(std::move(pComDropBomb));
      }
      break;

    case 'B': // DropBombDecorator
      {
        std::unique_ptr<CommandDropBombDecorator> pComDropBomb =
            std::make_unique<CommandDropBombDecorator>(
              FindPlane(),vecDynamicObj,bombsNumber,score
              );
        macroCommand.addCommand(std::move(pComDropBomb));
      }
      break;

    default:
      break;
  }
}

void SBomber::DrawFrame() {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

  for (size_t i = 0; i < vecDynamicObj.size(); i++) {
    if (vecDynamicObj[i] != nullptr) {
      vecDynamicObj[i]->Draw();
    }
  }

  for (size_t i = 0; i < vecStaticObj.size(); i++) {
    if (vecStaticObj[i] != nullptr) {
      vecStaticObj[i]->Draw();
    }
  }

  ScreenSingleton::getInstance().GotoXY(0, 0);
  fps++;

  std::shared_ptr<LevelGUI> gui = FindLevelGUI();
  gui->SetParam(passedTime, fps, bombsNumber, score);
  gui->newTankMessage();
}

void SBomber::TimeStart() {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");
  startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void SBomber::TimeFinish() {
  finishTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  deltaTime = uint16_t(finishTime - startTime);
  passedTime += deltaTime;

  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " deltaTime = ", (int)deltaTime);
}

void SBomber::RunCommands()
{
  macroCommand.Run();
}

void SBomber::TankMessage()
{
    for(auto& a: vecStaticObj)
    {
        if(a->ClassID() == "Tank")
        {
            std::shared_ptr<Tank> tank =
                    std::dynamic_pointer_cast<Tank>(a);
            tank->sendMessage();
        }
    }
}

void CommandDeleteDynamicObj::Execute()
{
  auto it = vecDynamicObj.begin();
  for (; it != vecDynamicObj.end(); it++) {
    if (*it == obj) {
      vecDynamicObj.erase(it);
      break;
    }
  }
}

void CommandDeleteStaticObj::Execute()
{
  auto it = vecStaticObj.begin();
  for (; it != vecStaticObj.end(); it++) {
    if (*it == obj) {
      vecStaticObj.erase(it);
      break;
    }
  }
}

void CommandDropBomb::Execute() {
  if (bombsNumber > 0) {
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

    double x = plane->GetX() + 4;
    double y = plane->GetY() + 2;

    std::shared_ptr<Bomb> pBomb = std::make_unique<Bomb>();
    pBomb->SetDirection(0.3, 1);
    pBomb->SetSpeed(2);
    pBomb->SetPos(x, y);
    pBomb->SetWidth(widthCrater);

    vecDynamicObj.push_back(std::static_pointer_cast<DynamicObject>(pBomb));
    bombsNumber--;
    score -= Bomb::BombCost;
  }
}

void CommandDropBombDecorator::Execute() {
  if (bombsNumber > 0) {
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

    double x = plane->GetX() + 4;
    double y = plane->GetY() + 2;

    std::unique_ptr<Bomb> pBomb = std::make_unique<Bomb>();
    pBomb->SetDirection(0.3, 1);
    pBomb->SetSpeed(2);
    pBomb->SetPos(x, y);
    pBomb->SetWidth(widthCrater);

    std::shared_ptr<BombDecorator> pBombDecorator =
        std::make_shared<BombDecorator>(std::move(pBomb));

    vecDynamicObj.push_back(std::static_pointer_cast<DynamicObject>(pBombDecorator));
    bombsNumber--;
    score -= Bomb::BombCost;
  }
}

void ConcreteCheckImpl::CheckPlaneAndLevelGUI(
    std::shared_ptr<Plane> plane, std::shared_ptr<LevelGUI> levelGui, bool& exitFlag) {
  if (plane->GetX() > levelGui->GetFinishX()) {
    exitFlag = true;
  }
}

std::unique_ptr<MacroCommand> ConcreteCheckImpl::CheckBombsAndGround (
    std::vector<std::shared_ptr<Bomb>> vecBombs,
    const std::shared_ptr<Ground>& pGround,
    std::vector<std::shared_ptr<DynamicObject>>& vecDynamicObj,
    const std::vector<std::shared_ptr<DestroyableGroundObject>>& vecDestoyableObjects,
    std::vector<std::shared_ptr<GameObject>>& vecStaticObj,
    int16_t& score) {
  std::unique_ptr<MacroCommand> macroCommand = std::make_unique<MacroCommand>();
  const double y = pGround->GetY();
  for (size_t i = 0; i < vecBombs.size(); i++) {
    if (vecBombs[i]->GetY() >= y) {
      pGround->AddCrater(vecBombs[i]->GetX());

      macroCommand->addCommands(CheckDestroyableObjects(vecBombs[i],vecDestoyableObjects,vecStaticObj,score));

      std::unique_ptr<CommandDeleteDynamicObj> pDelDynObj =
          std::make_unique<CommandDeleteDynamicObj>(
            vecBombs[i], vecDynamicObj
            );
      macroCommand->addCommand(std::move(pDelDynObj));
    }
  }
  return macroCommand;
}

std::unique_ptr<MacroCommand> ConcreteCheckImpl::CheckDestroyableObjects(
    std::shared_ptr<Bomb> pBomb,
    const std::vector<std::shared_ptr<DestroyableGroundObject>>& vecDestoyableObjects,
    std::vector<std::shared_ptr<GameObject>>& vecStaticObj,
    int16_t& score) {
  std::unique_ptr<MacroCommand> macroCommand = std::make_unique<MacroCommand>();
  const double size = pBomb->GetWidth();
  const double size_2 = size / 2;
  for (size_t i = 0; i < vecDestoyableObjects.size(); i++) {
    const double x1 = pBomb->GetX() - size_2;
    const double x2 = x1 + size;
    if (vecDestoyableObjects[i]->isInside(x1, x2)) {
      score += vecDestoyableObjects[i]->GetScore();

      std::unique_ptr<CommandDeleteStaticObj> pComDelStatObj =
          std::make_unique<CommandDeleteStaticObj>(
            vecDestoyableObjects[i], vecStaticObj
            );
      macroCommand->addCommand(std::move(pComDelStatObj));
    }
  }
  return macroCommand;
}
