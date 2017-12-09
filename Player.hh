#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "Minion.hh"
#include "LevelParser.hh"
#include "Constants.hh"

#include <vector>

using namespace std;

class Player {
   int wallet;
   sf::Sprite menu;
   sf::Sprite slimie;
   sf::Sprite ghostie;
   sf::Sprite skullie;
   sf::Sprite batsie;

public:
   Player(int levelid);
   ~Player();
   void draw(sf::RenderTarget& target, sf::View view);
   bool buy(Minion m);
   void updatePosition(sf::View view);
};

#endif