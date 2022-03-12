
#include <memory>

class SBomber;

class SBomberPimpl
{
public:
    SBomberPimpl();
    ~SBomberPimpl() = default;

    void TimeStart();
    void ProcessKBHit(int amountInputtedCodes);
    void DrawFrame();
    void MoveObjects();
    void CheckObjects();
    void RunCommands();
    void TankMessage();
    void TimeFinish();
    bool GetExitFlag();
private:
    std::shared_ptr<SBomber> sbomber;
};
