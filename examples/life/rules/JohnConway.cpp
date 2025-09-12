#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  // todo: implement
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

int JohnConway::CountNeighbors(World& world, Point2D point) {
  // todo: implement
  int count = 0;
  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1; j <= 1; ++j)
    {
      if (i == 0 && j == 0) continue;
      if (world.Get({point.x + j, point.y +i})) ++count;
    }
  }
  return count;
}
