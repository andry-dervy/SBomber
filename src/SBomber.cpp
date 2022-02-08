
#include "MyTools.h"
#include "SBomber.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "House.h"
#include "ScreenSingleton.h"
#include "enums/CraterSize.h"
#include <chrono>
#include <thread>

#if defined(_WIN32) || defined(WIN32)
#include <conio.h>
#include <windows.h>
#endif

#include <cassert>

SBomber::SBomber()
  : exitFlag(false), startTime(0), finishTime(0), deltaTime(0), passedTime(0),
    fps(0), bombsNumber(10), score(0) {
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

  std::unique_ptr<Plane> pPlane {new Plane};
  pPlane->SetDirection(1, 0.1);
  pPlane->SetSpeed(4);
  pPlane->SetPos(5, 10);
  vecDynamicObj.push_back(std::move(pPlane));

  std::unique_ptr<LevelGUI> pGUI {new LevelGUI};
  pGUI->SetParam(passedTime, fps, bombsNumber, score);
  const uint16_t maxX = ScreenSingleton::getInstance().GetMaxX();
  const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
  const uint16_t offset = 3;
  const uint16_t width = maxX - 7;
  pGUI->SetPos(offset, offset);
  pGUI->SetWidth(width);
  pGUI->SetHeight(maxY - 4);
  pGUI->SetFinishX(offset + width - 4);
  vecStaticObj.push_back(std::move(pGUI));

  std::unique_ptr<Ground> pGr {new Ground};
  const uint16_t groundY = maxY - 5;
  pGr->SetPos(offset + 1, groundY);
  pGr->SetWidth(width - 2);
  vecStaticObj.push_back(std::move(pGr));

  std::unique_ptr<Tank> pTank{new Tank};
  pTank->SetWidth(13);
  pTank->SetPos(15, groundY - 1);
  vecStaticObj.push_back(std::move(pTank));

  pTank = std::unique_ptr<Tank>(new Tank);
  pTank->SetWidth(13);
  pTank->SetPos(35, groundY - 1);
  vecStaticObj.push_back(std::move(pTank));


  std::unique_ptr<House> pHouse {new House};
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
//  for (size_t i = 0; i < vecDynamicObj.size(); i++) {
//    if (vecDynamicObj[i] != nullptr) {
//      delete vecDynamicObj[i];
//    }
//  }

//  for (size_t i = 0; i < vecStaticObj.size(); i++) {
//    if (vecStaticObj[i] != nullptr) {
//      delete vecStaticObj[i];
//    }
//  }
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

  CheckPlaneAndLevelGUI();
  CheckBombsAndGround();
};

void SBomber::CheckPlaneAndLevelGUI() {
  if (FindPlane()->GetX() > FindLevelGUI()->GetFinishX()) {
    exitFlag = true;
  }
}

void SBomber::CheckBombsAndGround() {
  std::vector<std::shared_ptr<Bomb>> vecBombs = FindAllBombs();
  std::shared_ptr<Ground> pGround = FindGround();
  const double y = pGround->GetY();
  for (size_t i = 0; i < vecBombs.size(); i++) {
    if (vecBombs[i]->GetY() >= y) {
      pGround->AddCrater(vecBombs[i]->GetX());
      CheckDestoyableObjects(vecBombs[i]);
      DeleteDynamicObj(vecBombs[i]);
    }
  }
}

void SBomber::CheckDestoyableObjects(std::shared_ptr<Bomb> pBomb) {
  std::vector<std::shared_ptr<DestroyableGroundObject>> vecDestoyableObjects =
      FindDestoyableGroundObjects();
  const double size = pBomb->GetWidth();
  const double size_2 = size / 2;
  for (size_t i = 0; i < vecDestoyableObjects.size(); i++) {
    const double x1 = pBomb->GetX() - size_2;
    const double x2 = x1 + size;
    if (vecDestoyableObjects[i]->isInside(x1, x2)) {
      score += vecDestoyableObjects[i]->GetScore();
      DeleteStaticObj(vecDestoyableObjects[i]);
    }
  }
}

void SBomber::DeleteDynamicObj(std::shared_ptr<DynamicObject> pObj) {
  auto it = vecDynamicObj.begin();
  for (; it != vecDynamicObj.end(); it++) {
    if (*it == pObj) {
      vecDynamicObj.erase(it);
      break;
    }
  }
}

void SBomber::DeleteStaticObj(std::shared_ptr<GameObject> pObj) {
  auto it = vecStaticObj.begin();
  for (; it != vecStaticObj.end(); it++) {
    if (*it == pObj) {
      vecStaticObj.erase(it);
      break;
    }
  }
}

std::vector<std::shared_ptr<DestroyableGroundObject>> SBomber::FindDestoyableGroundObjects() const {
  std::vector<std::shared_ptr<DestroyableGroundObject>> vec;
  for (size_t i = 0; i < vecStaticObj.size(); i++) {
    if (vecStaticObj[i]->ClassID() == "Tank" ||
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

std::vector<std::shared_ptr<Bomb>> SBomber::FindAllBombs() const {
  std::vector<std::shared_ptr<Bomb>> vecBombs;

  for (size_t i = 0; i < vecDynamicObj.size(); i++) {
    if (vecDynamicObj[i]->ClassID() == "Bomb") {
      vecBombs.push_back(std::static_pointer_cast<Bomb>(vecDynamicObj[i]));
    }
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

    case 'b':
      DropBomb();
      break;

    case 'B':
      DropBomb();
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

  FindLevelGUI()->SetParam(passedTime, fps, bombsNumber, score);
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

void SBomber::DropBomb() {
  if (bombsNumber > 0) {
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

    std::shared_ptr<Plane> pPlane = FindPlane();
    double x = pPlane->GetX() + 4;
    double y = pPlane->GetY() + 2;

    std::shared_ptr<Bomb> pBomb = std::make_unique<Bomb>();
    pBomb->SetDirection(0.3, 1);
    pBomb->SetSpeed(2);
    pBomb->SetPos(x, y);
    pBomb->SetWidth(SMALL_CRATER_SIZE);

    vecDynamicObj.push_back(std::static_pointer_cast<DynamicObject>(pBomb));
    bombsNumber--;
    score -= Bomb::BombCost;
  }
}
