#include "enemy.hpp"

unique_ptr<Texture> Enemy::s_tex1 = nullptr;
unique_ptr<Texture> Enemy::s_tex2 = nullptr;
bool Enemy::s_loaded1 = false;
bool Enemy::s_loaded2 = false;

Enemy::Enemy(Vector2u patrolA, Vector2u patrolB, TextureType type)
    : m_type(type)
    , m_pathIdx(0)
    , m_speedPx(40.f)
    , m_animFrame(0)
    , m_animTimer(0.f)
    , m_forward(true)
{
    (void)patrolA;  // Suppress unused parameter warning
    (void)patrolB;  // Suppress unused parameter warning
    ensureTexture();
    
    if (m_type == Enemy1 && s_tex1) {
        m_sprite.emplace(*s_tex1);
    } else if (m_type == Enemy2 && s_tex2) {
        m_sprite.emplace(*s_tex2);
    }
    
    if (m_sprite) {
        m_sprite->setTextureRect(IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));
        m_sprite->setScale({1.5f, 1.5f}); 
        m_sprite->setOrigin({TILE_SIZE / 2.f, TILE_SIZE / 2.f}); 
    }
    
    m_posPx = Vector2f(
        static_cast<float>(patrolA.x * TILE_SIZE) + TILE_SIZE / 2.f,
        static_cast<float>(patrolA.y * TILE_SIZE) + TILE_SIZE / 2.f
    );
    if (m_sprite) {
        m_sprite->setPosition(m_posPx);
    }
}

void Enemy::ensureTexture() {
    if (m_type == Enemy1 && !s_loaded1) {
        s_tex1 = make_unique<Texture>();
        string path = string(RESOURCE_DIR) + "/texture/enemies/enemy1.png";
        if (!s_tex1->loadFromFile(path)) {
            cerr << "Errore caricamento texture enemy1: " << path << endl;
            s_tex1.reset();
        } else {
            s_loaded1 = true;
        }
    } else if (m_type == Enemy2 && !s_loaded2) {
        s_tex2 = make_unique<Texture>();
        string path = string(RESOURCE_DIR) + "/texture/enemies/enemy2.png";
        if (!s_tex2->loadFromFile(path)) {
            cerr << "Errore caricamento texture enemy2: " << path << endl;
            s_tex2.reset();
        } else {
            s_loaded2 = true;
        }
    }
}

void Enemy::setPath(const deque<Vector2u>& path) {
    m_path = path;
    m_pathIdx = 0;
    
    if (!m_path.empty()) {
        m_posPx = Vector2f(
            static_cast<float>(m_path[0].x * TILE_SIZE) + TILE_SIZE / 2.f,
            static_cast<float>(m_path[0].y * TILE_SIZE) + TILE_SIZE / 2.f
        );
        if (m_sprite) {
            m_sprite->setPosition(m_posPx);
        }
    }
}

void Enemy::update(float dt) {
    if (m_path.empty() || m_pathIdx >= m_path.size()) {
        return;
    }
    
    Vector2u targetTile = m_path[m_pathIdx];
    Vector2f targetPx(
        static_cast<float>(targetTile.x * TILE_SIZE) + TILE_SIZE / 2.f,
        static_cast<float>(targetTile.y * TILE_SIZE) + TILE_SIZE / 2.f
    );
    
    Vector2f delta = targetPx - m_posPx;
    float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    
    bool movingRight = delta.x > 0.1f; 
    bool movingLeft = delta.x < -0.1f;
    
    if (distance < 0.5f) {
        m_posPx = targetPx;
        m_pathIdx++;
    } else {
        Vector2f direction = delta / distance;
        float step = m_speedPx * dt;
        
        if (step >= distance) {
            m_posPx = targetPx;
            m_pathIdx++;
        } else {
            m_posPx += direction * step;
        }
    }
    
    m_animTimer += dt;
    if (m_animTimer >= ANIM_SPEED) {
        m_animTimer -= ANIM_SPEED;
        m_animFrame = (m_animFrame + 1) % ANIM_FRAMES;
    }
    
    if (m_sprite) {
        int row = 0;
        if (movingLeft) {
            row = 1;
        } else if (movingRight) {
            row = 0;
        } else {
            row = m_forward ? 0 : 1;
        }
        
        m_sprite->setTextureRect(IntRect(
            {m_animFrame * TILE_SIZE, row * TILE_SIZE},
            {TILE_SIZE, TILE_SIZE}
        ));
        m_sprite->setPosition(m_posPx);
    }
}

void Enemy::draw(RenderTarget& target) const {
    if (m_sprite) {
        target.draw(*m_sprite);
    }
}

FloatRect Enemy::bounds() const {
    if (m_sprite) {
        return m_sprite->getGlobalBounds();
    }
    return FloatRect();
}

bool Enemy::atEnd() const {
    return m_pathIdx >= m_path.size();
}

void Enemy::reverse() {
    if (m_path.empty()) return;
    
    std::reverse(m_path.begin(), m_path.end());
    m_pathIdx = 0;
    
    m_forward = !m_forward;
    
    if (m_sprite) {
        int row = m_forward ? 0 : 1;
        m_sprite->setTextureRect(IntRect(
            {m_animFrame * TILE_SIZE, row * TILE_SIZE},
            {TILE_SIZE, TILE_SIZE}
        ));
    }
}

Vector2u Enemy::currentTile() const {
    return Vector2u(
        static_cast<unsigned int>(m_posPx.x / TILE_SIZE),
        static_cast<unsigned int>(m_posPx.y / TILE_SIZE)
    );
}
