#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
#include <cmath>
#include <algorithm>
using namespace std;

// hexagonal grid heuristic from Red Blob Games
// Calculates distance using hex grid movement rules
static float heuristic(const Point2D& a, const Point2D& b) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  return (std::abs(dx) + std::abs(dy) + std::abs(dx + dy)) / 2.0f;
}

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path.... tracks how we reached each tile.
  queue<Point2D> frontier;     // to store next ones to visit... queue of positions to explore bfs
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // tracks visited nodes... use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  //Start BFS at the cat’s current position
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  // borderExit will store the path endpoint if a border is found
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  // BFS Loop
  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    // Pop current point, mark as visited
    Point2D current = frontier.front();
    frontier.pop();
    frontierSet.erase(current);
    visited[current] = true;

    // If  current space is a border then found a valid exit path
    if (w->catWinsOnSpace(current)) {
      borderExit = current;
      break;
    }

    // Explore valid neighboring tiles
    // Add them to the BFS queue and record how we got there (cameFrom)
    for (auto& next : World::neighbors(current)) {
      if (!w->isValidPosition(next)) continue;
      if (visited[next]) continue;
      if (frontierSet.count(next)) continue;
      if (w->getContent(next)) continue;

      cameFrom[next] = current;
      frontier.push(next);
      frontierSet.insert(next);
    }
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move

  // If a border was reached... backtrack through cameFrom to build the path
  //  Reverse the path so it goes from cat  to border
  std::vector<Point2D> path;
  if (borderExit != Point2D::INFINITE) {
    Point2D step = borderExit;
    while (step != catPos) {
      path.push_back(step);
      step = cameFrom[step];
    }
    std::reverse(path.begin(), path.end());
  }
  return path;
}
