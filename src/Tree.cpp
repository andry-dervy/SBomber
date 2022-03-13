#include "Tree.h"
#include <iostream>
#include <chrono>
#include <cassert>
#include "ScreenSingleton.h"


void SmallTree::Grow(Tree* tree)
{
    assert(tree != nullptr);

    tree->setState(std::make_shared<MiddleTree>());
}

void SmallTree::Draw(double x, double y) const
{
    ScreenSingleton::getInstance().SetColor(CC_Cyan);
    ScreenSingleton::getInstance().GotoXY(x, y - 3);
    std::cout << " #####";
    ScreenSingleton::getInstance().GotoXY(x, y - 2);
    std::cout << "#######";
    ScreenSingleton::getInstance().GotoXY(x, y - 1);
    std::cout << "   #";
    ScreenSingleton::getInstance().GotoXY(x,y);
    std::cout << "   #";
}

void MiddleTree::Grow(Tree* tree)
{
    assert(tree != nullptr);

    tree->setState(std::make_shared<BigTree>());
}

void MiddleTree::Draw(double x, double y) const
{
    ScreenSingleton::getInstance().SetColor(CC_Cyan);
    ScreenSingleton::getInstance().GotoXY(x, y - 4);
    std::cout << "  ###";
    ScreenSingleton::getInstance().GotoXY(x, y - 3);
    std::cout << " #####";
    ScreenSingleton::getInstance().GotoXY(x, y - 2);
    std::cout << "#######";
    ScreenSingleton::getInstance().GotoXY(x, y - 1);
    std::cout << "   #";
    ScreenSingleton::getInstance().GotoXY(x,y);
    std::cout << "   #";
}

void BigTree::Grow(Tree* tree)
{
}

void BigTree::Draw(double x, double y) const
{
    ScreenSingleton::getInstance().SetColor(CC_Cyan);
    ScreenSingleton::getInstance().GotoXY(x, y - 5);
    std::cout << "   #";
    ScreenSingleton::getInstance().GotoXY(x, y - 4);
    std::cout << "  ###";
    ScreenSingleton::getInstance().GotoXY(x, y - 3);
    std::cout << " #####";
    ScreenSingleton::getInstance().GotoXY(x, y - 2);
    std::cout << "#######";
    ScreenSingleton::getInstance().GotoXY(x, y - 1);
    std::cout << "   #";
    ScreenSingleton::getInstance().GotoXY(x,y);
    std::cout << "   #";
}

Tree::Tree()
    :changeMoment(std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::high_resolution_clock::now().time_since_epoch()).count()),
     currentTime(changeMoment),
     stateTree(std::make_unique<SmallTree>()) {}

void Tree::setState(std::shared_ptr<StateTree> astateTree)
{
    stateTree = astateTree;
}

void Tree::Move(uint16_t time)
{
    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if( (currentTime - changeMoment) > 3000)
    {
        changeMoment = currentTime;
        stateTree->Grow(this);
    }
}

void Tree::Draw() const
{
    stateTree->Draw(GetX(),GetY());
}

std::string Tree::ClassID() const
{
    return "Tree";
}
