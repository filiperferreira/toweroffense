#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Constants.h"
#include <string>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

class LevelParser {
   int SPRITE_SIZE = 64;
   int sizeX, sizeY;
   int spawnx = 0, spawny = 0;
   map<char, sf::Texture> textureMap;
   vector<sf::Texture> texture;
   vector<vector<sf::Sprite>> tile;
   //vector<Tower> tower;
   deque<sf::Vector2f> positions;

private:
   template<typename Out>
   void split(const string &s, char delim, Out result);
   vector<string> split(const string &s, char delim);
public:
   ~LevelParser();
   LevelParser();
   void loadLevel(string level);
   int mapSizeX();
   int mapSizeY();
   void setLevelPosition(char curTile, int x, int y);
   sf::Vector2f getMinionSpawnPos();
   deque<sf::Vector2f> getPath();
   sf::Sprite getTile(int x, int y);
   void parse(ifstream* mapFile);
   vector<string> parseLine(ifstream* mapFile);
   void parseLevel(ifstream* mapFile);
};

#endif