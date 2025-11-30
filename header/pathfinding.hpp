#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <SFML/System/Vector2.hpp>
#include <deque>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <limits>
#include "engine.hpp"

using namespace std; 
using namespace sf;

class GridProvider {
public:
    virtual ~GridProvider() = default;
    
    virtual Vector2u size() const = 0;
    
    virtual bool isBlocked(Vector2u tile) const = 0;
};

deque<Vector2u> astar(const GridProvider& grid, 
                                Vector2u start, 
                                Vector2u goal);

#endif // PATHFINDING_HPP
