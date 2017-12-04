#include "LevelParser.h"

int SPRITE_SIZE = 64;
int sizeX, sizeY;
int spawnx = 0, spawny = 0;
map<char, sf::Texture> textureMap;
vector<sf::Texture> texture;
vector<vector<sf::Sprite>> tile;
//vector<Tower> tower;
deque<sf::Vector2f> positions;

LevelParser::LevelParser() {
    texture = vector<sf::Texture>(3);
    texture[0].loadFromFile("resources/textures/grass.png");
    texture[1].loadFromFile("resources/textures/ground.png");
    texture[2].loadFromFile("resources/textures/towertile.png");

    textureMap['X'] = texture[0];
    textureMap['O'] = texture[1];
    textureMap['S'] = texture[1]; //spawn position is S
    textureMap['T'] = texture[2];
}

void LevelParser::loadLevel(string level) {
    ifstream mapFile;

    mapFile.open(level);
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
                    //tower.push_back(Tower(1, 128.0));
                }
            }
        }

        mapFile.close();
    }
    else {
        cerr << "Error opening level data." << endl;
    }
}

int LevelParser::mapSizeX() {
    return sizeX;
}

int LevelParser::mapSizeY() {
    return sizeY;
}

void LevelParser::setLevelPosition(char curTile, int x, int y){
    if (curTile == 'S'){
        spawnx = x;
        spawny = y;
    }
    if (curTile == 'O'){
        positions.push_back(sf::Vector2f(x*SPRITE_SIZE, y*SPRITE_SIZE));
    }
}

int LevelParser::getSpawnPosX(){
    return spawnx*SPRITE_SIZE;
}

int LevelParser::getSpawnPosY(){
    return spawny*SPRITE_SIZE;
}

deque<sf::Vector2f> LevelParser::getPath(){
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

sf::Sprite LevelParser::getTile(int x, int y) {
    return tile[x][y];
}

LevelParser::~LevelParser(){
}
