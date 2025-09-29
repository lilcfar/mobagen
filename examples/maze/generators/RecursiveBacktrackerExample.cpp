#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>


void breakWall(World* w, const Point2D& from, const Point2D& to) {
  Point2D delta = to - from;

  if (delta.x == 1) {
    w->SetEast(from, false);
    w->SetWest(to, false);
  } else if (delta.x == -1) {
    w->SetWest(from, false);
    w->SetEast(to, false);
  } else if (delta.y == 1) {
    w->SetSouth(from, false);
    w->SetNorth(to, false);
  } else if (delta.y == -1) {
    w->SetNorth(from, false);
    w->SetSouth(to, false);
  }
}


bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement this
  int sideOver2 = w->GetSize() / 2;

  if (stack.empty()) {
    Point2D start = randomStartPoint(w);
    if (start.x == INT_MAX) return false;

    visited[start.y + sideOver2][start.x + sideOver2] = true;
    w->SetNodeColor(start, Color::Black);
    stack.push_back(start);
    return true;
  }

  Point2D current = stack.back();
  std::vector<Point2D> visitables = getVisitables(w, current);

  if (!visitables.empty()) {
    Point2D chosen;
    if (visitables.size() == 1)
      chosen = visitables[0];
    else
      chosen = visitables[std::rand() % visitables.size()];

    breakWall(w, current, chosen);

    visited[chosen.y + sideOver2][chosen.x + sideOver2] = true;
    w->SetNodeColor(chosen, Color::Black);
    stack.push_back(chosen);
  } else {
    stack.pop_back();
  }

  return true;
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  // todo: implement this
  std::vector<Point2D> directions = {
    {0, -1}, // UP
    {1, 0},  // RIGHT
    {0, 1},  // DOWN
    {-1, 0}  // LEFT
  };

  for (const Point2D& d : directions) {
    Point2D np = p + d;
    int x = np.x + sideOver2;
    int y = np.y + sideOver2;

    if (x >= 0 && x < w->GetSize() && y >= 0 && y < w->GetSize()) {
      if (!visited[y][x]) {
        visitables.push_back(np);
      }
    }
  }

  return visitables;
}
