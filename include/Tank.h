#pragma once

#include <stdint.h>
#include <memory>
#include <chrono>

#include "DestroyableGroundObject.h"
#include <Mediator.h>

struct TankMessage
{
    double x, y;
    std::string text;
};

class Tank : public DestroyableGroundObject
{
public:
    Tank() {
        time_pnt = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
	bool  isInside(double x1, double x2) const override;

	inline uint16_t GetScore() const override { return score; }

	void Draw() const override;
    std::string ClassID() const override;
    void addMediator(std::shared_ptr<Mediator>& amediator);
    void sendMessage();

private:
    std::shared_ptr<Mediator> mediator;
	const uint16_t score = 30;
    uint64_t time_pnt;
};

