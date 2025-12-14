#include "fruit.hpp"

Fruit::Fruit(){
    Vector2f startPosition(400, 300);
    sprite.setSize(Vector2f(20,20));
    sprite.setFillColor(Color::Red);  // fallback
    sprite.setPosition(startPosition);
} 

void Fruit::setPosition(Vector2f newPosition){
    sprite.setPosition(newPosition);
}

void Fruit::setTexture(const Texture* tex, int frames){
    texture = tex;
    frameCount = (frames > 0 ? frames : 3);
    sprite.setTexture(texture, true); 
    sprite.setFillColor(Color::White);
    pickRandomFrame();
}

const RectangleShape& Fruit::getSprite() const{
    return sprite;
}

void Fruit::pickRandomFrame(){
    if (!texture || frameCount <= 0) return;

    const auto tw = texture->getSize().x;
    const auto th = texture->getSize().y;

    const unsigned frameW = tw / static_cast<unsigned>(frameCount);
    const unsigned frameH = th; 

    static thread_local std::mt19937 rng(std::random_device{}());
    uniform_int_distribution<int> dist(0, static_cast<int>(frameCount) - 1);
    currentFrame = dist(rng);
    const int left = static_cast<int>(currentFrame * frameW);

    sprite.setTextureRect(IntRect(Vector2i(left, 0), Vector2i(static_cast<int>(frameW), static_cast<int>(frameH))));

    sprite.setSize(Vector2f(25.f, 25.f));
}