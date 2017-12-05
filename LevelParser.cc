#include "LevelParser.h"

int sizeX, sizeY;
sf::Vector2f minionSpawn;
map<char, sf::Texture> textureMap;
vector<sf::Texture> texture;
vector<vector<sf::Sprite>> tile;
//vector<Tower> tower;
deque<sf::Vector2f> positions;
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
        minionSpawn = sf::Vector2f(x*SPRITE_SIZE, y*SPRITE_SIZE);
    }
    if (curTile == PATH){
        positions.push_back(sf::Vector2f(x*SPRITE_SIZE, y*SPRITE_SIZE));
    }
}

sf::Vector2f LevelParser::getMinionSpawnPos(){
    return minionSpawn;
}

deque<sf::Vector2f> LevelParser::getPath(){
    deque<sf::Vector2f> np;
    float a, b;
    sf::Vector2f init = getMinionSpawnPos();
    for(deque<sf::Vector2f>::iterator i=positions.begin(); i!=positions.end(); i++) {
        a = (*i).x;
        b = (*i).y;
        np.push_back(sf::Vector2f(init.x, init.y));
        if (a == init.x && b != init.y){
            for(float i = init.y; i <= b; i+=0.1){
                np.push_back(sf::Vector2f(a, i));
            }
        }else if (a != init.x && b == init.y){
            for(float i = init.x; i <= a; i+=0.1){
                np.push_back(sf::Vector2f(i, b));
            }
        }
        init = sf::Vector2f(a, b);
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