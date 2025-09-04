#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to match the heading of neighbors = Average velocity
  Vector2f velocitySum = Vector2f::zero();
  int count = 0;

  // todo: add your code here to align each boid in a neighborhood
  // hint: iterate over the neighborhood
  for (Boid* neighbor : neighborhood) {
    if (!neighbor) continue;

    velocitySum += neighbor->getVelocity();
    count++;
  }

  if (count == 0) return Vector2f::zero(); // no allign force
  Vector2f averageVelocity = velocitySum / count;

  return averageVelocity;
}
