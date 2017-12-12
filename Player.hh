#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "Minion.hh"
#include "LevelParser.hh"
#include "Constants.hh"
#include <string>
#include <vector>

using namespace std;

class Player {
   int wallet = 0;
   sf::Sprite menu;
   sf::Sprite slimie;
   sf::Sprite ghostie;
   sf::Sprite skullie;
   sf::Sprite batsie;
   sf::Text walletText;
   sf::Font MyFont;
   LevelParser level;

public:
   Player(int levelid);
   ~Player();
   int getWallet();
   void setLevel(LevelParser level);
   void draw(sf::RenderTarget& target);
   bool buy(Minion m);
};

#endif