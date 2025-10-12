#include "Cat.h"
#include "World.h"

// Calls generatePath() from Agent.cpp
// the cat moves to the first step in the path

Point2D Cat::Move(World* world) {
  auto path = generatePath(world);
  if (path.empty()) {
    return world->getCat();
  }
  return path.front();
}
