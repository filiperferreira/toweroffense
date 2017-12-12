#include "Minion.hh"

Minion::Minion() {
}

float speed, health, initialHealth;
sf::Sprite minionSprite;
deque<pair<sf::Vector2f,sf::Vector2f>> movements;
sf::RectangleShape healthBar;
int price;
int OFFSET = 10;

Minion::Minion(string minionType, LevelParser lp) {
    minionSprite.setTexture(*(TextureManager::getTexture(minionType)));
    minionSprite.setPosition(lp.getMinionSpawnPos().x+OFFSET, lp.getMinionSpawnPos().y+OFFSET);
    healthBar.setPosition(minionSprite.getPosition().x, minionSprite.getPosition().y);
    movements = lp.getPath();
}

void Minion::draw(sf::RenderTarget& target){
    target.draw(minionSprite);
    target.draw(healthBar);
}

sf::Vector2f Minion::normalize(sf::Vector2f& source){
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f (source.x / length, source.y / length);
    else
        return source;
}

//moves from point a to b
bool Minion::moveTo(sf::Time timeElapsed, sf::Vector2f from, sf::Vector2f to){
    sf::Vector2f pos = to - from;
    sf::Vector2f dir = normalize(pos);
    sf::Vector2f w = minionSprite.getPosition() - sf::Vector2f(OFFSET,OFFSET) - to;
    float delta = timeElapsed.asSeconds() * 60 * speed*0.1;
    if (from.y == to.y && from.x < to.x){
        //printf("move forwards\n");
        minionSprite.move( delta * dir.x, delta * dir.y );
    }else if (from.y == to.y && from.x >= to.x){
        //printf("move backwards\n");
        minionSprite.move( delta * dir.x, delta * -dir.y);
    }else if (from.x == to.x && from.y < to.y){
        //printf("move down\n");
        minionSprite.move( 0.f, delta  * dir.y );
    }else if (from.x == to.x && from.y >=  to.y){
        //printf("move up\n");
        minionSprite.move( delta * -dir.x, delta  * dir.y );
    }
    healthBar.setPosition(minionSprite.getPosition().x, minionSprite.getPosition().y);
    return int(w.x) == int(w.y) && int(w.y) == 0;
}

void Minion::move(sf::Time timeElapsed) {
    if (movements.size() > 0){
        pair<sf::Vector2f,sf::Vector2f> point = movements.front();
        sf::Vector2f from = point.first;
        sf::Vector2f to = point.second;
        if (moveTo(timeElapsed, from, to)){
            movements.pop_front(); //change direction
        }
    }
}

void Minion::setSpeed(float spd){
    speed = spd;
}

void Minion::setPrice(int p){
    price = p;
}

int Minion::getPrice(){
    return price;
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

void Minion::updateHealthBar(float dam){
    if (health > 35)
        healthBar.setSize(sf::Vector2f(35, 5));
    else
        healthBar.setSize(sf::Vector2f(health, 5));
    healthBar.setFillColor(sf::Color(112, 87, 156));
    if (initialHealth*0.5 >= health){
        healthBar.setFillColor(sf::Color(255,165,0));
    }
    if (initialHealth*0.2 >= health){
        healthBar.setFillColor(sf::Color(255,0,0));
    }
}

sf::Vector2f Minion::getPosition(){
    return minionSprite.getPosition();
}

void Minion::damage(float dam){
    if (isAlive()){
        health -= dam;
        if (health < 0) health = 0;
        updateHealthBar(dam);
    }
}

bool Minion::isAlive(){
    return health > 0;
}

Minion::~Minion(){
}
