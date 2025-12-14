#include "pathfinding.hpp"

struct Node {
    Vector2u pos;
    float g;
    float h; 
    float f; 
    
    Node(Vector2u p, float gCost, float hCost) 
        : pos(p), g(gCost), h(hCost), f(gCost + hCost) {}
    
    bool operator>(const Node& other) const {
        if (f == other.f)
            return h > other.h;  // tie-break: nodo con H più basso
        return f > other.f;
    }
};

struct Vector2uHash {
    size_t operator()(const Vector2u& v) const {
        return hash<unsigned int>()(v.x) ^ (hash<unsigned int>()(v.y) << 1);
    }
};

/**
 * Calcola la distanza di Manhattan tra due punti sulla griglia
 */
static float manhattan(Vector2u a, Vector2u b) {
    return static_cast<float>(abs(static_cast<int>(a.x) - static_cast<int>(b.x)) + 
                              abs(static_cast<int>(a.y) - static_cast<int>(b.y)));
}

bool Engine::isBlocked(Vector2u tile) const {
    if (tile.x >= mapSize.x || tile.y >= mapSize.y) {
        return true;
    }
    
    return levelMap.isWallCell(tile.x, tile.y);
}

deque<Vector2u> astar(const GridProvider& grid, 
                      Vector2u start, 
                      Vector2u goal) {
    if (grid.isBlocked(start) || grid.isBlocked(goal)) {
        return {};
    }
    
    if (start == goal) {
        return {start};
    }
    
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<Vector2u, float, Vector2uHash> gScore;
    unordered_map<Vector2u, Vector2u, Vector2uHash> cameFrom;
    
    openSet.emplace(start, 0.f, manhattan(start, goal));
    gScore[start] = 0.f;
    
    const Vector2i directions[] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };
    
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        
        if (current.pos == goal) {
            deque<Vector2u> path;
            Vector2u step = goal;
            
            while (step != start) {
                path.push_front(step);
                step = cameFrom[step];
            }
            path.push_front(start);
            
            return path;
        }
        
        for (const auto& dir : directions) {
            Vector2i neighborPos(
                static_cast<int>(current.pos.x) + dir.x,
                static_cast<int>(current.pos.y) + dir.y
            );
            
            if (neighborPos.x < 0 || neighborPos.y < 0) continue;
            
            Vector2u neighbor(static_cast<unsigned int>(neighborPos.x), 
                                  static_cast<unsigned int>(neighborPos.y));
            
            if (grid.isBlocked(neighbor)) continue;
            
            float tentativeG = current.g + 1.f;
            
            auto it = gScore.find(neighbor);
            if (it != gScore.end() && tentativeG >= it->second) {
                continue;
            }
            
            cameFrom[neighbor] = current.pos;
            gScore[neighbor] = tentativeG;
            
            float h = manhattan(neighbor, goal);
            openSet.emplace(neighbor, tentativeG, h);
        }
    }
    
    return {};
}

Vector2u findNearestGreenPatrol(
    const Vector2u& yellowStart,
    const vector<Vector2u>& greenPatrols
) {
    // Se non ci sono punti verdi, ritorna il punto giallo stesso (caso fallback)
    if (greenPatrols.empty()) {
        return yellowStart;
    }
    
    // Trova il punto verde con la distanza di Manhattan minima
    Vector2u nearest = greenPatrols[0];
    float minDistance = manhattan(yellowStart, greenPatrols[0]);
    
    for (size_t i = 1; i < greenPatrols.size(); ++i) {
        float distance = manhattan(yellowStart, greenPatrols[i]);
        if (distance < minDistance) {
            minDistance = distance;
            nearest = greenPatrols[i];
        }
    }
    
    return nearest;
}
