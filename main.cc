#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <math.h>
#include "TextureManager.h"

using namespace std;

class Tower {

    float range, damage, health;

    sf::Vector2f initialPosition;
    sf::Clock attackClock;

    public:
    Tower(float d, float r, sf::Vector2f p, float h): damage(d), range(r), initialPosition(p), health(h) {
        attackClock.restart();
    }

    sf::Vector2f getInitialPosition(){
        return initialPosition;
    }

    float getRange(){
        return range;
    }

    void resetCooldown(){

            attackClock.restart();



    }

    float getDamage(){
        return damage;
    }

    int getClock(){
        return attackClock.getElapsedTime().asMilliseconds();
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
    vector<sf::Vector2f> lockedPositions;
    deque<sf::Vector2f> positions;

    public:
    levelMap() {
        levelFiles[1] = "resources/maps/level3.bin";

        texture = vector<sf::Texture>(4);
        texture[0].loadFromFile("resources/textures/grass.png");
        texture[1].loadFromFile("resources/textures/ground.png");
        texture[2].loadFromFile("resources/textures/towertile.png");
        texture[3].loadFromFile("resources/textures/water.png");


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
                        tower.push_back(Tower(10, 128.0, tile[i][j].getPosition(), 2000));
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
            positions.push_back(sf::Vector2f(x*SPRITE_SIZE, y*SPRITE_SIZE));
        }
    }

    int getSpawnPosX(){
        return spawnx*SPRITE_SIZE;
    }

    int getSpawnPosY(){
        return spawny*SPRITE_SIZE;
    }

    deque<sf::Vector2f> getPositions(){
        deque<sf::Vector2f> np;
        float a, b, initA = getSpawnPosX(), initB = getSpawnPosY();
        for(deque<sf::Vector2f>::iterator i=positions.begin(); i!=positions.end(); i++) {
            a = (*i).x;
            b = (*i).y;
            np.push_back(sf::Vector2f(initA, initB));
            if (a == initA && b != initB){
                for(float i = initB; i <= b; i+=0.1){
                    np.push_back(sf::Vector2f(a, i));
                }
            }else if (a != initA && b == initB){
                for(float i = initA; i <= a; i+=0.1){
                    np.push_back(sf::Vector2f(i, b));
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

      vector<Tower> getTowers(){
        return tower;
    }
};

class Minion {
    float speed, health, initialHealth;
    int price;
    sf::Sprite minionSprite;
    deque<sf::Vector2f> movements;
    sf::Vector2f currPosition;
    sf::RectangleShape healthBar;
    string type;
    sf::Clock attackClock;

    public:
    Minion(string minionType, levelMap lm) {
        type = minionType;
        minionSprite.setTexture(*(TextureManager::getTexture(minionType)));
        minionSprite.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        healthBar.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        movements = lm.getPositions();
        attackClock.restart().asSeconds();
    }

    void draw(sf::RenderTarget& target){
        target.draw(minionSprite);
        target.draw(healthBar);
    }

    void move(sf::Time timeElapsed) {
        if (movements.size() > 0){
            sf::Vector2f point = movements.front();
            setPosition(point);
            movements.pop_front();
            minionSprite.setPosition(getPosition().x, getPosition().y);
            healthBar.setPosition(minionSprite.getPosition().x, minionSprite.getPosition().y);
        }
    }
    string getMinionType(){
        return type;
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
    void restartCooldown(){
        attackClock.restart();
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

    void setPosition(sf::Vector2f position){
        currPosition = position;
    }

    sf::Vector2f getPosition(){
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

class mapElementsController {
    vector<Tower> towers;
    vector<Minion> minions;
    vector<sf::Vector2f> towerLockedPositions;
    vector<bool> isLckd;
    sf::Clock towerAttackClock;
    public:
    mapElementsController(){
    }
    int getMinionNumber(){
        return minions.size();
    }

    void addMinion(Minion m){
        minions.push_back(m);

    }

    void addTower(Tower t){
        towers.push_back(t);
    }
    void addTowers(vector<Tower> t){
        towers = t;
    }
    float distanceCheck(Tower t,Minion m){
        float x1, x2, y1, y2;
        x1 =t.getInitialPosition().x;
        y1 =t.getInitialPosition().y;
        x2 =m.getPosition().x;
        y2 =m.getPosition().y;
        return sqrt(pow((x1-x2),2) + pow((y1-y2),2 ));

    }
    void towerAttack(Tower& t,Minion& m){

        if(t.getClock() >= 500 && distanceCheck(t,m) <= t.getRange()){

            m.damage(t.getDamage());

            t.resetCooldown();



        }
    }



    void updateLockedPositions(){

        sf::Vector2f pos1,pos2,pos3,pos4;
        vector<sf::Vector2f> newLockedPositions;

        int SPRITE_SIZE = 64;

        for(vector<Tower>::iterator it=towers.begin(); it!=towers.end(); it++){// temporary way to get the adjacent tiles do a tower disregarding type of tile. to do:lock only ground tiles.

            pos1 = sf::Vector2f(it->getInitialPosition().x + SPRITE_SIZE, it->getInitialPosition().y);
            pos2 = sf::Vector2f(it->getInitialPosition().x - SPRITE_SIZE, it->getInitialPosition().y);
            pos3 = sf::Vector2f(it->getInitialPosition().x , it->getInitialPosition().y + SPRITE_SIZE);
            pos4 = sf::Vector2f(it->getInitialPosition().x , it->getInitialPosition().y - SPRITE_SIZE);

            newLockedPositions.push_back(pos1);
            newLockedPositions.push_back(pos2);
            newLockedPositions.push_back(pos3);
            newLockedPositions.push_back(pos4);

        }
        towerLockedPositions = newLockedPositions;
    }

    void removeMinion(int m){
        minions.erase(minions.begin() + m );
        isLckd.erase(isLckd.begin() + m);
    }

    void updateElements( sf::Time timeElapsed ){
        isLckd = vector<bool>(minions.size());
        for (int i = 0; i < minions.size(); i++) {
            isLckd[i]=false;
            for(int j = 0; j < towerLockedPositions.size(); j++){
                if((minions[i].getPosition().x > (towerLockedPositions[j].x -0.1) && minions[i].getPosition().x < (towerLockedPositions[j].x + 0.1)) &&
                    (minions[i].getPosition().y > (towerLockedPositions[j].y -0.1) && minions[i].getPosition().y < (towerLockedPositions[j].y + 0.1)) &&
                    ((minions[i].getMinionType() == "blue") || (minions[i].getMinionType() == "green"))){
                        isLckd[i]=true;
                        }


            }
            if(!isLckd[i] && minions[i].isAlive()) {
                minions[i].move(timeElapsed);
            }
            if(!minions[i].isAlive()){
                removeMinion(i);
            }
        }
        for(int j= 0 ; j< towers.size(); j++){
                for(int i= 0 ; i< minions.size(); i++){
                    towerAttack(towers[j],minions[i]);
                }

        }
    }

    void drawMinions(sf::RenderWindow& window, sf::Time timeElapsed){

        updateElements(timeElapsed);
        for (int i = 0; i < minions.size(); i++) {
            minions[i].draw(window);
        }
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

    mapElementsController elementsController; //now with 100% more avatar

    elementsController.addTowers(thisLevel.getTowers());
    //elementsController.setTowerLockedPositions(thisLevel.getLockedPos());

    // needs to name the texture and then call getTexture by its name
    TextureManager::loadTexture("blue", "resources/textures/blue.png");
    TextureManager::loadTexture("green", "resources/textures/green.png");
    TextureManager::loadTexture("purple", "resources/textures/purple.png");
    TextureManager::loadTexture("yellow", "resources/textures/yellow.png");





    while (window.isOpen()) {

        sf::Event event;
        sf::Time timeElapsed;

        elementsController.updateLockedPositions();

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

                    elementsController.addMinion(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    Minion m("green", thisLevel);
                    m.setSpeed(10);
                    m.setHealth(100);

                    elementsController.addMinion(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    Minion m("purple", thisLevel);
                    m.setSpeed(10);
                    m.setHealth(100);

                    elementsController.addMinion(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    Minion m("yellow", thisLevel);
                    m.setSpeed(10);
                    m.setHealth(100);


                    elementsController.addMinion(m);

                }
            }
        }

        window.clear();

        window.setView(view);


        for (int i = 0; i < thisLevel.mapSizeX(); i++) {
            for (int  j = 0; j < thisLevel.mapSizeY(); j++) {
                window.draw(thisLevel.getTile(i, j));
            }
        }


        elementsController.drawMinions(window, timeElapsed);

        window.display();



    }

    return 0;
}


