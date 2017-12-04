#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <string>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>

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

public:
   ~LevelParser();
   LevelParser();
   void loadLevel(string level);
   int mapSizeX();
   int mapSizeY();
   void setLevelPosition(char curTile, int x, int y);
   int getSpawnPosX();
   int getSpawnPosY();
   deque<sf::Vector2f> getPath();
   sf::Sprite getTile(int x, int y);
};

#endif