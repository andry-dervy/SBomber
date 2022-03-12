#ifndef TREE_H
#define TREE_H

#include <memory>
#include "DynamicObject.h"

class Tree;

class StateTree
{
public:
    virtual ~StateTree() {}
    virtual void Grow(Tree* tree) = 0;
    virtual void Draw(double x, double y) const = 0;
};

class SmallTree: public StateTree
{
public:
    void Grow(Tree* tree) override;
    void Draw(double x, double y) const override;
};

class MiddleTree: public StateTree
{
public:
    void Grow(Tree* tree) override;
    void Draw(double x, double y) const override;
};

class BigTree: public StateTree
{
public:
    void Grow(Tree* tree) override;
    void Draw(double x, double y) const override;
};

class Tree: public DynamicObject
{
private:
    uint64_t changeMoment, currentTime;
    std::shared_ptr<StateTree> stateTree;
public:
    Tree();
    void setState(std::shared_ptr<StateTree> astateTree);
    void Move(uint16_t time) override;
    void Draw() const override;
    virtual std::string ClassID() const;

};

#endif // TREE_H
