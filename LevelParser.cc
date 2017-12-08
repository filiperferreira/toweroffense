#include "LevelParser.h"

int sizeX, sizeY;
sf::Vector2f minionSpawn;
map<char, sf::Texture> textureMap;
vector<sf::Texture> texture;
vector<vector<sf::Sprite>> tile;
//vector<Tower> tower;
vector<string> minions;
vector<string> towers;
deque< pair<sf::Vector2f,sf::Vector2f> > minionMovements;

using namespace LevelConstants;

LevelParser::LevelParser() {
    texture = vector<sf::Texture>(3);
    texture[0].loadFromFile(GRASS_TILE);
    texture[1].loadFromFile(GROUND_TILE);
    texture[2].loadFromFile("resources/textures/towertile.png");

    textureMap[GRASS]  = texture[0];
    textureMap[PATH]   = texture[1];
    textureMap[SPAWN]  = texture[1]; //spawn position is S
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
    minionMovements = parsePath(mapFile);
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
    if (curTile == SPAWN){
        minionSpawn = sf::Vector2f(x*SPRITE_SIZE, y*SPRITE_SIZE);
    }
}

sf::Vector2f LevelParser::getMinionSpawnPos(){
    return minionSpawn;
}

deque< pair<sf::Vector2f,sf::Vector2f> > LevelParser::getPath(){
    return minionMovements;
}

deque<pair<sf::Vector2f,sf::Vector2f>> LevelParser::parsePath(ifstream* mapFile){
    deque<pair<sf::Vector2f,sf::Vector2f>> movements;
    int x1, y1, x2, y2;
    while (!(*mapFile).eof()){
        std::vector<std::string> pos = parseLine(mapFile);
        if (!pos.empty()){        
            x1 = atoi(pos[0].c_str());
            y1 = atoi(pos[1].c_str());
            x2 = atoi(pos[2].c_str());
            y2 = atoi(pos[3].c_str());

            sf::Vector2f from = sf::Vector2f(x1*SPRITE_SIZE, y1*SPRITE_SIZE);
            sf::Vector2f to = sf::Vector2f(x2*SPRITE_SIZE, y2*SPRITE_SIZE);
            pair <sf::Vector2f, sf::Vector2f> point (from, to);
            movements.push_back(point);
        }  
    }
    return movements;
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