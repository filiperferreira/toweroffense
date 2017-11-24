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

class levelMap {
    int SPRITE_SIZE = 60;
    int sizeX, sizeY;
    int spawnx = 0, spawny = 0;
    map<int, string> levelFiles;
    map<char, sf::Texture> textureMap;
    vector<sf::Texture> texture;
    vector<vector<sf::Sprite>> tile;
    deque<sf::Vector2i> positions;

    public:
    levelMap() {
        levelFiles[1] = "resources/maps/level2.bin";

        texture = vector<sf::Texture>(2);
        texture[0].loadFromFile("resources/textures/grass.png");
        texture[1].loadFromFile("resources/textures/ground.png");

        textureMap['X'] = texture[0];
        textureMap['O'] = texture[1];
        textureMap['S'] = texture[1]; //spawn position is S
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
    float speed;
    sf::Sprite minionSprite;
    TextureManager textureManager;
    deque<sf::Vector2i> movements;

    public:
        Minion(string minionType, levelMap lm) {
            minionSprite.setTexture(textureManager.getTexture("resources/textures/"+minionType+".png"));
            minionSprite.setPosition(lm.getSpawnPosX(), lm.getSpawnPosY());
            movements = lm.getPositions();
        }

        void draw(sf::RenderTarget& target){
            target.draw(minionSprite);
        }

        void move(float timer){
            if (movements.size() > 0){
                sf::Vector2i point = movements.front();
                movements.pop_front();
                minionSprite.setPosition(point.x, point.y);
                sf::sleep(sf::seconds(getSpeed()/1000));
            }
        }

        void setSpeed(float spd){
            speed = spd;
        }

        float getSpeed(){
            return speed;
        }
};

int main() {
    sf::Clock timer;
    float minionTime = timer.restart().asSeconds();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Tower Offense");

    sf::View view(sf::Vector2f(512, 512), sf::Vector2f(768, 672));
    view.setViewport(sf::FloatRect(0.25, 0, 0.75, 0.875));

    TextureManager tm;

    levelMap thisLevel;
    thisLevel.loadLevel(1);
    
    Minion m("blue", thisLevel);
    m.setSpeed(10);
    

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
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
