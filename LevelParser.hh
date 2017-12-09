#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "Constants.hh"
#include <string>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <utility> 

using namespace std;

class LevelParser {
   int sizeX, sizeY;
   sf::Vector2f minionSpawn, guardianPos;
   map<char, sf::Texture> textureMap;
   vector<sf::Texture> texture;
   vector<vector<sf::Sprite>> tile;
   //vector<Tower> tower;
   vector<string> minions;
   vector<string> towers;
   deque< pair<sf::Vector2f,sf::Vector2f> > minionMovements;

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
   sf::Vector2f getGuardianPos();
   deque< pair<sf::Vector2f,sf::Vector2f> > getPath();
   sf::Sprite getTile(int x, int y);
   void parse(ifstream* mapFile);
   vector<string> parseLine(ifstream* mapFile);
   void parseLevel(ifstream* mapFile);
   deque< pair<sf::Vector2f,sf::Vector2f> > parsePath(ifstream* mapFile);
};

#endif