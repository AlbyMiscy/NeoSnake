#ifndef LEVELMAP_HPP
#define LEVELMAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace sf;
using namespace std;

/**
 * LevelMap gestisce una mappa di livello basata su CSV multi-layer esportati da Tiled.
 * Ogni livello contiene 4 layer:
 * - mapCSV_wall.csv: muri e collisioni
 * - mapCSV_grass.csv: sfondo (erba)
 * - mapCSV_grass_accessories.csv: accessori sfondo (erba alta, pietre, ecc.)
 * - mapCSV_enemies_patrols.csv: punti di pattuglia per i nemici
 */
class LevelMap {
public:
    explicit LevelMap(float cellSize = 20.f);

    /**
     * Carica tutti i layer da una cartella, es: "resources/maps/level01"
     * @param folderPath percorso della cartella contenente i CSV
     * @param tilesetTexture texture del tileset (160x160, 8x8 tile da 20x20)
     * @return true se il caricamento è riuscito
     */
    bool loadFromFolder(const string& folderPath,
                        const Texture& tilesetTexture);

    /**
     * Disegna la mappa nell'ordine: sfondo, accessori, muri
     */
    void draw(RenderTarget& target) const;

    /**
     * Restituisce la larghezza del mondo in pixel
     */
    float getWorldWidth() const;

    /**
     * Costruisce un VertexArray per lo sfondo da un singolo file CSV
     * Metodo statico utilizzabile senza istanziare LevelMap
     * @param va VertexArray da riempire
     * @param csvPath percorso del file CSV
     * @param tilesetTexture texture del tileset
     * @param cellSize dimensione della cella in pixel (default 20.f)
     * @return true se il caricamento è riuscito
     */
    static bool buildBackgroundFromCsv(VertexArray& va,
                                       const string& csvPath,
                                       const Texture& tilesetTexture,
                                       float cellSize = 20.f);

    /**
     * Restituisce l'altezza del mondo in pixel
     */
    float getWorldHeight() const;

    /**
     * Restituisce la dimensione di una cella
     */
    float getCellSize() const { return m_cellSize; }

    /**
     * Verifica se una cella è un muro
     * @param cx coordinata x della cella
     * @param cy coordinata y della cella
     * @return true se la cella è un muro
     */
    bool isWallCell(std::size_t cx, std::size_t cy) const;

    /**
     * Converte coordinate mondo in coordinate cella
     */
    Vector2i worldToCell(const Vector2f& p) const;

    /**
     * Restituisce i punti di pattuglia A per i nemici
     */
    const vector<Vector2u>& getPatrolPointsA() const { return m_patrolA; }

    /**
     * Restituisce i punti di pattuglia B per i nemici
     */
    const vector<Vector2u>& getPatrolPointsB() const { return m_patrolB; }

    /**
     * Restituisce le dimensioni della griglia (numero di celle)
     */
    Vector2u getGridSize() const;

private:
    float m_cellSize;

    // Layer di celle (ID dei tile) caricati dai CSV
    vector<vector<int>> m_wallGrid;
    vector<vector<int>> m_grassGrid;
    vector<vector<int>> m_grassAccGrid;
    vector<vector<int>> m_enemyPatrolGrid;

    // VertexArray per disegno efficiente dei 3 layer grafici
    VertexArray m_grassVA;
    VertexArray m_grassAccVA;
    VertexArray m_wallVA;

    const Texture* m_tileset = nullptr;

    // Punti di pattuglia per i nemici
    vector<Vector2u> m_patrolA;
    vector<Vector2u> m_patrolB;

    /**
     * Carica una griglia da un file CSV
     * @param csvPath percorso del file CSV
     * @return griglia caricata (righe x colonne)
     */
    static vector<vector<int>> loadCsvGrid(const string& csvPath);

    /**
     * Costruisce un VertexArray da una griglia di tile
     * @param grid griglia di ID tile
     * @param va VertexArray da riempire
     */
    void buildVertexArrayFromGrid(const vector<vector<int>>& grid,
                                  VertexArray& va);

    /**
     * Estrae i punti di pattuglia dalla griglia dei nemici
     * Valori diversi indicano tipo di punto (1 = patrol A, 2 = patrol B)
     */
    void extractPatrolPoints();
};

#endif // LEVELMAP_HPP
