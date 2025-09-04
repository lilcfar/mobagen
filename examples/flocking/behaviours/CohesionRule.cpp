#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f sum{0.f, 0.f};
  int count = 0;

  // todo: add your code here to make a force towards the center of mass
  // hint: iterate over the neighborhood
  // find center of mass
  for (Boid* neighbor: neighborhood) {
    if (!neighbor) continue;
    if (neighbor == boid) continue;

    const Vector2f pos = neighbor->getPosition();
    sum.x += pos.x;
    sum.y += pos.y;
    count++;
  }

  if (count == 0) return Vector2f{0.f, 0.f};


  Vector2f com{ sum.x / static_cast<float>(count), sum.y / static_cast<float>(count) };

  const Vector2f myPos = boid->getPosition();     // adjust accessor if needed
  Vector2f cohesionForce{ com.x - myPos.x, com.y - myPos.y };


  return cohesionForce;
}
