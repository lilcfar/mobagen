#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to avoid boids too close
  Vector2f separatingForce = Vector2f::zero();
  float desiredDistance = desiredMinimalDistance;

  //    // todo: implement a force that if neighbor(s) enter the radius, moves the boid away from it/them
  //    if (!neighborhood.empty()) {
  //        Vector2f position = boid->transform.position;
  //        int countCloseFlockmates = 0;
  //        // todo: find and apply force only on the closest mates
  //    }
  for (Boid* neighbor : neighborhood) {
    if (!neighbor || neighbor == boid) continue;

    const Vector2f& neighborPosition = neighbor->transform.position;
    Vector2f awayPosition = boid->transform.position - neighborPosition;
    float distSqr =  awayPosition.x * awayPosition.x + awayPosition.y * awayPosition.y;
    // setting reaction boundary
    if (distSqr < desiredDistance * desiredDistance ) {
      float dist = std::sqrt(distSqr);
      float weight = (desiredDistance * desiredDistance) / desiredDistance;
      awayPosition.x /= dist;
      awayPosition.y /= dist;

      separatingForce.x += awayPosition.x * weight;
      separatingForce.y += awayPosition.y * weight;
    }
  }

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
