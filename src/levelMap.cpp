#include "levelMap.hpp"

LevelMap::LevelMap(float cellSize)
    : m_cellSize(cellSize)
    , m_grassVA(PrimitiveType::Triangles)
    , m_grassAccVA(PrimitiveType::Triangles)
    , m_wallVA(PrimitiveType::Triangles)
{
}

bool LevelMap::loadFromFolder(const string& folderPath,
                               const Texture& tilesetTexture) {
    m_tileset = &tilesetTexture;
    
    // Disabilita smoothing per evitare texture bleeding
    const_cast<Texture*>(m_tileset)->setSmooth(false);

    // path dei 4 CSV
    string wallPath = folderPath + "/mapCSV_wall.csv";
    string grassPath = folderPath + "/mapCSV_grass.csv";
    string grassAccPath = folderPath + "/mapCSV_grass_accessories.csv";
    string enemyPatrolPath = folderPath + "/mapCSV_enemies_patrols.csv";

    // Caricamento delle le griglie
    m_wallGrid = loadCsvGrid(wallPath);
    m_grassGrid = loadCsvGrid(grassPath);
    m_grassAccGrid = loadCsvGrid(grassAccPath);
    m_enemyPatrolGrid = loadCsvGrid(enemyPatrolPath);

    // Verifica che almeno una griglia sia stata caricata
    if (m_wallGrid.empty() || m_grassGrid.empty()) {
        cerr << "Errore: impossibile caricare i CSV dalla cartella " << folderPath << endl;
        return false;
    }

    // VertexArray per i 3 layer grafici
    buildVertexArrayFromGrid(m_grassGrid, m_grassVA);
    buildVertexArrayFromGrid(m_grassAccGrid, m_grassAccVA);
    buildVertexArrayFromGrid(m_wallGrid, m_wallVA);

    // Estrazione dei punti di pattuglia
    extractPatrolPoints();

    return true;
}

void LevelMap::draw(RenderTarget& target) const {
    if (!m_tileset) return;

    // Disegna in ordine: sfondo, accessori, muri
    target.draw(m_grassVA, m_tileset);
    target.draw(m_grassAccVA, m_tileset);
    target.draw(m_wallVA, m_tileset);
}

float LevelMap::getWorldWidth() const {
    if (m_wallGrid.empty()) return 0.f;
    return static_cast<float>(m_wallGrid[0].size()) * m_cellSize;
}

float LevelMap::getWorldHeight() const {
    if (m_wallGrid.empty()) return 0.f;
    return static_cast<float>(m_wallGrid.size()) * m_cellSize;
}

bool LevelMap::isWallCell(size_t cx, size_t cy) const {
    // Se fuori bounds, considera come muro per sicurezza
    if (cy >= m_wallGrid.size()) return true;
    if (cx >= m_wallGrid[cy].size()) return true;
    
    // Un tile con ID != -1 è un muro (Tiled usa -1 per "nessun tile")
    return m_wallGrid[cy][cx] != -1;
}

Vector2i LevelMap::worldToCell(const Vector2f& p) const {
    int cx = static_cast<int>(floor(p.x / m_cellSize));
    int cy = static_cast<int>(floor(p.y / m_cellSize));
    return { cx, cy };
}

Vector2u LevelMap::getGridSize() const {
    if (m_wallGrid.empty()) return {0, 0};
    unsigned int rows = static_cast<unsigned int>(m_wallGrid.size());
    unsigned int cols = static_cast<unsigned int>(m_wallGrid[0].size());
    return {cols, rows};
}

vector<vector<int>> LevelMap::loadCsvGrid(const string& csvPath) {
    vector<vector<int>> grid;
    ifstream file(csvPath);

    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire il file " << csvPath << endl;
        return grid;
    }

    string line;
    while (getline(file, line)) {
        // Ignora righe vuote
        if (line.empty()) continue;

        vector<int> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            // Rimozione di eventuali spazi bianchi
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);

            if (!cell.empty()) {
                int value = stoi(cell);
                row.push_back(value);
            }
        }

        if (!row.empty()) {
            grid.push_back(row);
        }
    }

    file.close();
    return grid;
}

void LevelMap::buildVertexArrayFromGrid(const vector<vector<int>>& grid,
                                        VertexArray& va) {
    va.clear();

    if (grid.empty() || !m_tileset) return;

    // Dimensioni del tileset: 160x160, 8x8 tile da 20x20
    const int tilesetWidth = 8;  // numero di tile per riga
    const int tileSizeInTexture = 20;

    size_t rows = grid.size();
    
    for (size_t row = 0; row < rows; ++row) {
        size_t cols = grid[row].size();
        
        for (size_t col = 0; col < cols; ++col) {
            int tileId = grid[row][col];

            // Tiled usa -1 per "nessun tile", quindi ignoriamo valori < 0
            if (tileId < 0) continue;

            // Posizione del tile nel mondo
            float worldX = static_cast<float>(col) * m_cellSize;
            float worldY = static_cast<float>(row) * m_cellSize;

            // Calcola la posizione del tile nel tileset
            // tileId = index nel tileset (0-based)
            int tileX = tileId % tilesetWidth;
            int tileY = tileId / tilesetWidth;

            // Applica un inset di 0.5 pixel per evitare texture bleeding
            const float inset = 0.5f;
            float texLeft = static_cast<float>(tileX * tileSizeInTexture) + inset;
            float texTop = static_cast<float>(tileY * tileSizeInTexture) + inset;
            float texRight = static_cast<float>((tileX + 1) * tileSizeInTexture) - inset;
            float texBottom = static_cast<float>((tileY + 1) * tileSizeInTexture) - inset;

            // Crea due triangoli per formare un quad (6 vertici)
            // Primo triangolo: top-left, top-right, bottom-left
            Vertex v1, v2, v3, v4, v5, v6;
            
            // Triangolo 1
            v1.position = Vector2f(worldX, worldY);
            v1.texCoords = Vector2f(texLeft, texTop);
            
            v2.position = Vector2f(worldX + m_cellSize, worldY);
            v2.texCoords = Vector2f(texRight, texTop);
            
            v3.position = Vector2f(worldX, worldY + m_cellSize);
            v3.texCoords = Vector2f(texLeft, texBottom);
            
            // Triangolo 2
            v4.position = Vector2f(worldX + m_cellSize, worldY);
            v4.texCoords = Vector2f(texRight, texTop);
            
            v5.position = Vector2f(worldX + m_cellSize, worldY + m_cellSize);
            v5.texCoords = Vector2f(texRight, texBottom);
            
            v6.position = Vector2f(worldX, worldY + m_cellSize);
            v6.texCoords = Vector2f(texLeft, texBottom);
            
            va.append(v1);
            va.append(v2);
            va.append(v3);
            va.append(v4);
            va.append(v5);
            va.append(v6);
        }
    }
}

void LevelMap::extractPatrolPoints() {
    m_patrolA.clear();
    m_patrolB.clear();

    if (m_enemyPatrolGrid.empty()) return;

    size_t rows = m_enemyPatrolGrid.size();
    
    for (size_t row = 0; row < rows; ++row) {
        size_t cols = m_enemyPatrolGrid[row].size();
        
        for (size_t col = 0; col < cols; ++col) {
            int value = m_enemyPatrolGrid[row][col];

            // Valori dal CSV Tiled
            // 48 = patrol A, 56 = patrol B
            if (value == 48) {
                m_patrolA.emplace_back(static_cast<unsigned int>(col), 
                                       static_cast<unsigned int>(row));
            } else if (value == 56) {
                m_patrolB.emplace_back(static_cast<unsigned int>(col), 
                                       static_cast<unsigned int>(row));
            }
        }
    }
}

bool LevelMap::buildBackgroundFromCsv(VertexArray& va,
                                      const string& csvPath,
                                      const Texture& tilesetTexture,
                                      float cellSize) {
    va.clear();
    va.setPrimitiveType(PrimitiveType::Triangles);

    // Carica la griglia dal CSV
    vector<vector<int>> grid = loadCsvGrid(csvPath);
    
    if (grid.empty()) {
        cerr << "Errore: impossibile caricare lo sfondo da " << csvPath << endl;
        return false;
    }

    // Calcola le dimensioni del tileset automaticamente dalla texture
    const auto texSize = tilesetTexture.getSize();
    const int tileSizeInTexture = static_cast<int>(cellSize); // Assume tile size = cell size
    const int tilesetWidth = static_cast<int>(texSize.x) / tileSizeInTexture;

    size_t rows = grid.size();
    
    for (size_t row = 0; row < rows; ++row) {
        size_t cols = grid[row].size();
        
        for (size_t col = 0; col < cols; ++col) {
            int tileId = grid[row][col];

            // Tiled usa -1 per "nessun tile"
            if (tileId < 0) continue;

            // Posizione del tile nel mondo
            float worldX = static_cast<float>(col) * cellSize;
            float worldY = static_cast<float>(row) * cellSize;

            // Calcola la posizione del tile nel tileset
            int tileX = tileId % tilesetWidth;
            int tileY = tileId / tilesetWidth;

            // Applica un inset di 0.5 pixel per evitare texture bleeding
            const float inset = 0.5f;
            float texLeft = static_cast<float>(tileX * tileSizeInTexture) + inset;
            float texTop = static_cast<float>(tileY * tileSizeInTexture) + inset;
            float texRight = static_cast<float>((tileX + 1) * tileSizeInTexture) - inset;
            float texBottom = static_cast<float>((tileY + 1) * tileSizeInTexture) - inset;

            // Crea due triangoli per formare un quad (6 vertici)
            Vertex v1, v2, v3, v4, v5, v6;
            
            // Triangolo 1
            v1.position = Vector2f(worldX, worldY);
            v1.texCoords = Vector2f(texLeft, texTop);
            
            v2.position = Vector2f(worldX + cellSize, worldY);
            v2.texCoords = Vector2f(texRight, texTop);
            
            v3.position = Vector2f(worldX, worldY + cellSize);
            v3.texCoords = Vector2f(texLeft, texBottom);
            
            // Triangolo 2
            v4.position = Vector2f(worldX + cellSize, worldY);
            v4.texCoords = Vector2f(texRight, texTop);
            
            v5.position = Vector2f(worldX + cellSize, worldY + cellSize);
            v5.texCoords = Vector2f(texRight, texBottom);
            
            v6.position = Vector2f(worldX, worldY + cellSize);
            v6.texCoords = Vector2f(texLeft, texBottom);
            
            va.append(v1);
            va.append(v2);
            va.append(v3);
            va.append(v4);
            va.append(v5);
            va.append(v6);
        }
    }

    return true;
}
