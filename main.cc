#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>

using namespace std;

class TextureManager {
    map<string, sf::Texture> textures;

    public:
    TextureManager(): textures(){}

    const sf::Texture& getTexture(const std::string& filename) {
        for(map<string, sf::Texture>::const_iterator it = textures.begin(); it != textures.end(); ++it){
            if( filename == it->first ){
                return it->second;
            }
        }

        sf::Texture image;
        if(image.loadFromFile( filename )){
            textures[filename] = image;
            return textures[filename];
        }

        textures[filename] = image;
        return textures[filename];
    }
};

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
    float speed, health;
    sf::Sprite minionSprite;
    TextureManager textureManager;
    deque<sf::Vector2i> movements;
    sf::Vector2i currPosition;
    sf::RectangleShape healthBar;

    public:
    Minion(string minionType, levelMap lm) {
        minionSprite.setTexture(textureManager.getTexture("resources/textures/"+minionType+".png"));
        minionSprite.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        healthBar.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
        movements = lm.getPositions();
    }

    void draw(sf::RenderTarget& target){
        target.draw(minionSprite);
        target.draw(healthBar);
    }

    void move(float timer){
        if (movements.size() > 0){
            sf::Vector2i point = movements.front();
            setPosition(point);
            movements.pop_front();
            minionSprite.setPosition(getPosition().x, getPosition().y);
            healthBar.setPosition(getPosition().x, getPosition().y);
            sf::sleep(sf::seconds(getSpeed()/1000));
        }
    }

    void setSpeed(float spd){
        speed = spd;
    }

    float getSpeed(){
        return speed;
    }

    void setHealth(float hlt){
        health = hlt;
        healthBar.setSize(sf::Vector2f(hlt/4, 5));
        healthBar.setFillColor(sf::Color(100, 250, 50));
    }

    float getHealth(){
        return health;
    }

    void setPosition(sf::Vector2i position){
        currPosition = position;
    }

    sf::Vector2i getPosition(){
        return currPosition;
    }
};

int main() {
    sf::Clock timer;
    float minionTime = timer.restart().asSeconds();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Tower Offense");

    sf::View view(sf::Vector2f(384, 336), sf::Vector2f(768, 672));
    view.setViewport(sf::FloatRect(0.25, 0, 0.75, 0.875));
    sf::View menu(sf::Vector2f(), sf::Vector2f(256, 672));
    menu.setViewport(sf::FloatRect(0, 0, 0.25, 0.875));

    TextureManager tm;

    levelMap thisLevel;
    thisLevel.loadLevel(1);

    Minion m("blue", thisLevel);
    m.setSpeed(10);
    m.setHealth(100);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (view.getCenter().x - 384 > 0) {
                        view.move(-64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (view.getCenter().x + 384 < thisLevel.mapSizeX() * 64) {
                        view.move(64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    if (view.getCenter().y - 336 > 0) {
                        view.move(0, -64);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (view.getCenter().y + 336 < thisLevel.mapSizeY() * 64) {
                        view.move(0, 64);
                    }
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

        m.move(timer.getElapsedTime().asSeconds());
        m.draw(window);
        window.display();
    }

    return 0;
}
