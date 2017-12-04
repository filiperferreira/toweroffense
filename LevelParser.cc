#include "LevelParser.h"

int SPRITE_SIZE = 64;
int sizeX, sizeY;
int spawnx = 0, spawny = 0;
map<char, sf::Texture> textureMap;
vector<sf::Texture> texture;
vector<vector<sf::Sprite>> tile;
//vector<Tower> tower;
deque<sf::Vector2f> positions;
vector<string> tiles;
vector<string> paths;
vector<string> minions;
vector<string> towers;

using namespace LevelConstants;

LevelParser::LevelParser() {
    texture = vector<sf::Texture>(3);
    texture[0].loadFromFile(GRASS_TILE);
    texture[1].loadFromFile(GROUND_TILE);
    texture[2].loadFromFile("resources/textures/towertile.png");

    textureMap[GRASS]  = texture[0];
    textureMap[PATH]   = texture[1];
    textureMap[MINION] = texture[1]; //spawn position is S
    textureMap[TOWER]  = texture[2];
}

void LevelParser::loadLevel(string level) {
    ifstream mapFile;
    mapFile.open(level);
    if (mapFile.is_open()) {
        parse(&mapFile);
        mapFile.close();
    }else {
        cerr << "Error opening level data." << endl;
    }
}

void LevelParser::parse(ifstream* mapFile){
    tiles   = parseLine(mapFile);
    paths   = parseLine(mapFile);
    minions = parseLine(mapFile);
    towers  = parseLine(mapFile);
    parseLevel(mapFile);
}

/* parses the actual level layout */
void LevelParser::parseLevel(ifstream* mapFile) {
    (*mapFile) >> sizeX >> sizeY;
    tile = vector<vector<sf::Sprite>>(sizeX);
    for (int i = 0; i < sizeX; i++) {
        tile[i] = vector<sf::Sprite>(sizeY);
        for (int j = 0; j < sizeY; j++) {
            char curTile;

            (*mapFile) >> curTile;

            setLevelPosition(curTile, j, i);

            tile[i][j].setTexture(textureMap[curTile]);
            tile[i][j].setPosition(SPRITE_SIZE*j, SPRITE_SIZE*i);
        }
    }
}

/* parses a line with a space delimiter */
vector<string> LevelParser::parseLine(ifstream* mapFile){
    string line;
    std::getline((*mapFile), line);
    std::vector<std::string> chars = split(line, ' ');
    return chars;
}

int LevelParser::mapSizeX() {
    return sizeX;
}

int LevelParser::mapSizeY() {
    return sizeY;
}

void LevelParser::setLevelPosition(char curTile, int x, int y){
    if (curTile == MINION){
        spawnx = x;
        spawny = y;
    }
    if (curTile == PATH){
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


/*helper functions*/
template<typename Out>
void LevelParser::split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> LevelParser::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}