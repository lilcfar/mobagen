//
// Created by atolstenko on 2/9/2023.
//

#include "HexagonGameOfLife.h"
void HexagonGameOfLife::Step(World& world) {
  const int wrldSz = world.SideSize(); //nxn

  for (int y = 0; y < wrldSz; ++y) {
    for (int x = 0; x < wrldSz; ++x) {
      const bool alive = world.Get({x, y});
      const int  n = CountNeighbors(world, {x, y});

      const bool nextAlive = alive ? (n == 2 || n == 3) : (n == 3);

      world.SetNext({x, y}, nextAlive);
    }
  }

  world.SwapBuffers();
}

int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) {
  int count = 0;
  const int p = (point.y & 1);

  // beside
  if (world.Get({point.x - 1, point.y})) ++count;
  if (world.Get({point.x + 1, point.y})) ++count;

  //abouve
  if (world.Get({point.x + p - 1, point.y - 1})) ++count;
  if (world.Get({point.x + p,     point.y - 1})) ++count;

  // below
  if (world.Get({point.x + p - 1, point.y + 1})) ++count;
  if (world.Get({point.x + p,     point.y + 1})) ++count;

  return count;
}
