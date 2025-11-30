#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include <optional>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "resource_path.h"

using namespace std;
using namespace sf;

class Enemy {
public:
    enum TextureType {
        Enemy1,
        Enemy2
    };
    
    Enemy(Vector2u patrolA, Vector2u patrolB, TextureType type);
    
    void update(float dt);
    
    void draw(RenderTarget& target) const;
    
    FloatRect bounds() const;
    
    bool atEnd() const;
    
    void reverse();
    
    Vector2u currentTile() const;
    
    void setPath(const deque<Vector2u>& path);
    
private:
    void ensureTexture();
    
    TextureType m_type;
    
    static unique_ptr<Texture> s_tex1;
    static unique_ptr<Texture> s_tex2;
    static bool s_loaded1;
    static bool s_loaded2;
    
    optional<Sprite> m_sprite;
    
    Vector2f m_posPx;
    
    deque<Vector2u> m_path;
    size_t m_pathIdx;
    
    float m_speedPx;
    
    int m_animFrame;
    float m_animTimer;
    bool m_forward; 
    
    static constexpr int ANIM_FRAMES = 5;
    static constexpr float ANIM_SPEED = 0.1f;
    static constexpr int TILE_SIZE = 20;
};

#endif // ENEMY_HPP
