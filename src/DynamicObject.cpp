
#include "DynamicObject.h"

void DynamicObject::Move(uint16_t time)
{
  x += xDirection * speed * time * 0.001;
  y += yDirection * speed * time * 0.001;
}

std::vector<std::shared_ptr<DestroyableGroundObject>> DynamicObject::CheckDestroyableObjects(int16_t& score)
{
  std::vector<std::shared_ptr<DestroyableGroundObject>> vecDest;
  const double size = GetWidth();
  const double size_2 = size / 2;
  for (auto& obs: observers_)
  {
    const double x1 = GetX() - size_2;
    const double x2 = x1 + size;

    if(obs->HandleInsideCheck(x1,x2))
    {
      score += obs->GetScore();
      vecDest.push_back(obs);
    }
  }
  return vecDest;
}

void DynamicObject::AddObserver(std::shared_ptr<DestroyableGroundObject> observer) {
    if (observer) {
        observers_.push_back(observer);
    }
}

void DynamicObject::AddObservers(std::vector<std::shared_ptr<DestroyableGroundObject>>& observers) {
    if (!observers.empty()) {
        observers_.reserve(observers_.size() + observers.size());
        observers_.insert(observers_.end(),observers.begin(),observers.end());
    }
}

void DynamicObject::DelObserver(const std::shared_ptr<DestroyableGroundObject>& observer) {
    auto it = std::find(observers_.begin(), observers_.end(), observer);
    if (it != observers_.end()) {
        observers_.erase(it);
    }
}
