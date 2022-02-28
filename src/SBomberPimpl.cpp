
#include "SBomberPimpl.h"
#include "SBomber.h"

SBomberPimpl::SBomberPimpl():
    sbomber(std::make_unique<SBomber>(
            std::make_unique<ConcreteCheckImpl>())){}

void SBomberPimpl::TimeStart()
{
    if (sbomber != nullptr)
        sbomber->TimeStart();
}

void SBomberPimpl::ProcessKBHit(int amountInputtedCodes)
{
    if (sbomber != nullptr)
        sbomber->ProcessKBHit(amountInputtedCodes);
}

void SBomberPimpl::DrawFrame()
{
    if (sbomber != nullptr)
        sbomber->DrawFrame();
}

void SBomberPimpl::MoveObjects()
{
    if (sbomber != nullptr)
        sbomber->MoveObjects();
}

void SBomberPimpl::CheckObjects()
{
    if (sbomber != nullptr)
        sbomber->CheckObjects();
}

void SBomberPimpl::RunCommands()
{
    if (sbomber != nullptr)
        sbomber->RunCommands();
}

void SBomberPimpl::TankMessage()
{
    if (sbomber != nullptr)
        sbomber->TankMessage();
}

void SBomberPimpl::TimeFinish()
{
    if (sbomber != nullptr)
        sbomber->TimeFinish();
}

bool SBomberPimpl::GetExitFlag()
{
    if (sbomber != nullptr)
        return sbomber->GetExitFlag();
    else
        return true;
}

