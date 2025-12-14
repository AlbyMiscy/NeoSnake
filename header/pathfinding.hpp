#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <SFML/System/Vector2.hpp>
#include <deque>
#include <queue>
#include <unordered_map>
#include <vector>
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

/**
 * Esegue l'algoritmo A* per trovare il percorso più breve tra start e goal
 * @param grid interfaccia per verificare le celle bloccate
 * @param start posizione di partenza (coordinate griglia)
 * @param goal posizione di arrivo (coordinate griglia)
 * @return percorso come sequenza di celle, vuoto se non c'è percorso
 */
deque<Vector2u> astar(const GridProvider& grid, 
                      Vector2u start, 
                      Vector2u goal);

/**
 * Trova il punto verde (patrol end) più vicino a un punto giallo (patrol start)
 * Usa la distanza di Manhattan sulla griglia (NON A*)
 * @param yellowStart il punto giallo di partenza
 * @param greenPatrols la lista di tutti i punti verdi disponibili
 * @return il punto verde più vicino, o yellowStart se la lista è vuota
 */
Vector2u findNearestGreenPatrol(
    const Vector2u& yellowStart,
    const vector<Vector2u>& greenPatrols
);

#endif // PATHFINDING_HPP
