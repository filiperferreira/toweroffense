#include "Minion.h"

Minion::Minion() {
}

float speed, health, initialHealth;
sf::Sprite minionSprite;
deque<sf::Vector2f> movements;
sf::Vector2f currPosition;
sf::RectangleShape healthBar;

Minion::Minion(string minionType, sf::Vector2f initPos, deque<sf::Vector2f> initMovements) {
    minionSprite.setTexture(*(TextureManager::getTexture(minionType)));
    minionSprite.setPosition(initPos.x, initPos.y);
    healthBar.setPosition(initPos.x, initPos.y);
    movements = initMovements;
}

void Minion::draw(sf::RenderTarget& target){
    target.draw(minionSprite);
    target.draw(healthBar);
}

void Minion::move(sf::Time timeElapsed) {
    if (movements.size() > 0){
        sf::Vector2f point = movements.front();
        setPosition(point);
        movements.pop_front();
        minionSprite.setPosition(getPosition().x, getPosition().y);
        healthBar.setPosition(minionSprite.getPosition().x, minionSprite.getPosition().y);
    }
}

void Minion::setSpeed(float spd){
    speed = spd;
}

float Minion::getSpeed(){
    return speed;
}

void Minion::setHealth(float hlt){
    if (hlt > 0){
        health = hlt;
    }else{
        health = -hlt;
    }
    initialHealth = health; //unaltered after this!
    updateHealthBar(health);
}

float Minion::getHealth(){
    return health;
}

void Minion::updateHealthBar(float hlt){
    healthBar.setSize(sf::Vector2f(hlt/4, 5));
    healthBar.setFillColor(sf::Color(100, 250, 50));
    if (initialHealth*0.5 >= health){
        healthBar.setFillColor(sf::Color(255,165,0));
    }
    if (initialHealth*0.2 >= health){
        healthBar.setFillColor(sf::Color(255,0,0));
    }
}

void Minion::setPosition(sf::Vector2f position){
    currPosition = position;
}

sf::Vector2f Minion::getPosition(){
    return currPosition;
}

void Minion::damage(float dam){
    if (isAlive()){
        health -= dam;
        if (health < 0) health = 0;
        updateHealthBar(health);
    }
}

bool Minion::isAlive(){
    return health > 0;
}

Minion::~Minion(){
}
