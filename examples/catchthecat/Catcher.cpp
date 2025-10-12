#include "Catcher.h"
#include "World.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

// A* node structure
// g is cost from start
// f = g + heuristic     (total estimated cost)
struct Node {
  Point2D pos;
  float g, f;
  bool operator>(const Node& other) const { return f > other.f; }
};

// A* distance heuristic
// hexagonal grid heuristic from Red Blob Games
// Calculates distance using hex grid movement rules
// same as agenr.cpp
static float heuristic(const Point2D& a, const Point2D& b) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  return (std::abs(dx) + std::abs(dy) + std::abs(dx + dy)) / 2.0f;
}

Point2D Catcher::Move(World* w) {
  Point2D catPos = w->getCat();
  int boardSize = w->getWorldSideSize();

  // A* setup
  // open  is priority queue ordered by lowest f-cost
  // cameFrom  is how we got to each tile
  // gScore  is cost to reach a tile
  // closed  is  tiles already explored
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
  std::unordered_map<Point2D, Point2D> cameFrom;
  std::unordered_map<Point2D, float> gScore;
  std::unordered_set<Point2D> closed;

  // start search at cat current tile
  gScore[catPos] = 0.0f;
  open.push({catPos, 0.0f, 0.0f});


   Point2D bestTile = catPos;
   float bestHeuristic = 1e9;

   // A* search loop
    while (!open.empty()) {
      //take the lowest cost node from open
      Node current = open.top();
      open.pop();
      //skip if already visited
      if (closed.count(current.pos)) continue;
      closed.insert(current.pos);

      // Evaluate "blocking" potential for thetiles near cat
      // finds the empty tile closest to the center
      // prioritizes placing blocks near center early on

      // Boardside / 2: Gets the center coordinate of the world
      // std::max(std::abs(current.pos.x), std::abs(current.pos.y)): Measures how far current.pos is from the center in either direction
      // h = abs(center - distance_from_center): the smaller h is the closer we are to the center
      // If h < bestHeuristic... update best tile
      float h = std::abs(boardSize/2 - std::max(std::abs(current.pos.x), std::abs(current.pos.y)));
      if (!w->getContent(current.pos) && h < bestHeuristic) {
        bestHeuristic = h;
        bestTile = current.pos;
      }

      // neighbor exploration
      // for each neighboor: if is valid and empty it computes a neww g and f scores
      // and update the scoress and addto open queue
      //g(n) is actual cost from the start node to the current node n
      //h(n) the heuristic estimate of the cheapest path from node n to the goal
      // f(n) total estimated cost of the cheapest path through node n
      for (auto& n : World::neighbors(current.pos)) {
        if (!w->isValidPosition(n) || w->getContent(n)) continue;
        float tentativeG = gScore[current.pos] + 1.0f;
        if (!gScore.count(n) || tentativeG < gScore[n]) {
          cameFrom[n] = current.pos;
          gScore[n] = tentativeG;
          // value f is used in the priority queue (open) to decide which node to explore next
          // the one with the lowest total estimated cost
          float f = tentativeG + heuristic(n, catPos);
          open.push({n, tentativeG, f});
        }
      }
    }
   return bestTile;
}