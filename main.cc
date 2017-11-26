#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include "TextureManager.h"

using namespace std;

class Tower {
    int damage;
    float range;

    public:
    Tower(int d, float r): damage(d), range(r) {

    }
};

class levelMap {
    int SPRITE_SIZE = 64;
    int sizeX, sizeY;
    int spawnx = 0, spawny = 0;
    map<int, string> levelFiles;
    map<char, sf::Texture> textureMap;
    vector<sf::Texture> texture;
    vector<vector<sf::Sprite>> tile;
    vector<Tower> tower;
    deque<sf::Vector2i> positions;

    public:
    levelMap() {
        levelFiles[1] = "resources/maps/level3.bin";

        texture = vector<sf::Texture>(3);
        texture[0].loadFromFile("resources/textures/grass.png");
        texture[1].loadFromFile("resources/textures/ground.png");
        texture[2].loadFromFile("resources/textures/tower.png");

        textureMap['X'] = texture[0];
        textureMap['O'] = texture[1];
        textureMap['S'] = texture[1]; //spawn position is S
        textureMap['T'] = texture[2];
    }

    void loadLevel(int levelId) {
        ifstream mapFile;

        mapFile.open(levelFiles[levelId]);
        if (mapFile.is_open()) {
            mapFile >> sizeX >> sizeY;
            tile = vector<vector<sf::Sprite>>(sizeX);

            for (int i = 0; i < sizeX; i++) {
                tile[i] = vector<sf::Sprite>(sizeY);
                for (int j = 0; j < sizeY; j++) {
                    char curTile;

                    mapFile >> curTile;

                    setLevelPosition(curTile, j, i);

                    tile[i][j].setTexture(textureMap[curTile]);
                    tile[i][j].setPosition(SPRITE_SIZE*j, SPRITE_SIZE*i);

                    if (curTile == 'T') {
                        tower.push_back(Tower(1, 128.0));
                    }
                }
            }

            mapFile.close();
        }
        else {
            cerr << "Error opening level data." << endl;
        }
    }

    int mapSizeX() {
        return sizeX;
    }

    int mapSizeY() {
        return sizeY;
    }

    void setLevelPosition(char curTile, int x, int y){
        if (curTile == 'S'){
            spawnx = x;
            spawny = y;
        }
        if (curTile == 'O'){
            positions.push_back(sf::Vector2i(x*SPRITE_SIZE, y*SPRITE_SIZE));
        }
    }

    int getSpawnPosX(){
        return spawnx*SPRITE_SIZE;
    }

    int getSpawnPosY(){
        return spawny*SPRITE_SIZE;
    }

    deque<sf::Vector2i> getPositions(){
        deque<sf::Vector2i> np;
        int a, b, initA = getSpawnPosX(), initB = getSpawnPosY();
        for(deque<sf::Vector2i>::iterator i=positions.begin(); i!=positions.end(); i++) {
            a = (*i).x;
            b = (*i).y;
            np.push_back(sf::Vector2i(initA, initB));
            if (a == initA && b != initB){
                for(int i = initB; i <= b; i++){
                    np.push_back(sf::Vector2i(a, i));
                }
            }else if (a != initA && b == initB){
                for(int i = initA; i <= a; i++){
                    np.push_back(sf::Vector2i(i, b));
                }
            }
            initA = a;
            initB = b;
        }
        return np;
    }

    sf::Sprite getTile(int x, int y) {
        return tile[x][y];
    }
};

class Minion {
    float speed, health, initialHealth;
    int price;
    sf::Sprite minionSprite;
    deque<sf::Vector2i> movements;
    sf::Vector2i currPosition;
    sf::RectangleShape healthBar;

    public:
    Minion(string minionType, levelMap lm) {
        minionSprite.setTexture(*(TextureManager::getTexture(minionType)));
        minionSprite.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        healthBar.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        movements = lm.getPositions();
    }

    void draw(sf::RenderTarget& target){
        target.draw(minionSprite);
        target.draw(healthBar);
    }

    void move(sf::Time timeElapsed) {
        if (movements.size() > 0){
            sf::Vector2i point = movements.front();
            setPosition(point);
            movements.pop_front();
            minionSprite.setPosition(getPosition().x, getPosition().y);
            healthBar.setPosition(getPosition().x, getPosition().y);
        }
    }

    void setSpeed(float spd){
        speed = spd;
    }

    float getSpeed(){
        return speed;
    }

    void setHealth(float hlt){
        if (hlt > 0){
            health = hlt;
        }else{
            health = -hlt;
        }
        initialHealth = health; //unaltered after this!
        updateHealthBar(health);
    }

    float getHealth(){
        return health;
    }

    void updateHealthBar(float hlt){
        healthBar.setSize(sf::Vector2f(hlt/4, 5));
        healthBar.setFillColor(sf::Color(100, 250, 50));
        if (initialHealth*0.5 >= health){
            healthBar.setFillColor(sf::Color(255,165,0));
        }
        if (initialHealth*0.2 >= health){
            healthBar.setFillColor(sf::Color(255,0,0));
        }
    }

    void setPosition(sf::Vector2i position){
        currPosition = position;
    }

    sf::Vector2i getPosition(){
        return currPosition;
    }

    void damage(float dam){
        if (isAlive()){
            health -= dam;
            if (health < 0) health = 0;
            updateHealthBar(health);
        }
    }

    bool isAlive(){
        return health > 0;
    }
};

int main() {
    sf::Clock timer;
    float minionTime = timer.restart().asSeconds();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Tower Offense");
    sf::Clock clock;

    sf::View view(sf::Vector2f(512, 384), sf::Vector2f(1024, 768));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    /*sf::View menu(sf::Vector2f(), sf::Vector2f(256, 672));
    menu.setViewport(sf::FloatRect(0, 0, 0.25, 0.875));*/

    levelMap thisLevel;
    thisLevel.loadLevel(1);
    
    // needs to name the texture and then call getTexture by its name
    TextureManager::loadTexture("blue", "resources/textures/blue.png");

    vector<Minion> minions;

    while (window.isOpen()) {
        sf::Event event;
        sf::Time timeElapsed;

        timeElapsed = clock.restart();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (view.getCenter().x - 512 > 0) {
                        view.move(-64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (view.getCenter().x + 512 < thisLevel.mapSizeX() * 64) {
                        view.move(64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    if (view.getCenter().y - 384 > 0) {
                        view.move(0, -64);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (view.getCenter().y + 384 < thisLevel.mapSizeY() * 64) {
                        view.move(0, 64);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    Minion m("blue", thisLevel);
                    m.setSpeed(10);
                    m.setHealth(100);

                    minions.push_back(m);
                }
            }
        }

        window.clear();

        window.setView(view);
        for (int i = 0; i < thisLevel.mapSizeX(); i++) {
            for (int j = 0; j < thisLevel.mapSizeY(); j++) {
                window.draw(thisLevel.getTile(i, j));
            }
        }
        for (int i = 0; i < minions.size(); i++) {
            minions[i].move(timeElapsed);
            minions[i].draw(window);
        }
        window.display();
    }

    return 0;
}
