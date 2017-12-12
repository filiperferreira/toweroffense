#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "Constants.hh"

using namespace std;

class Tower {
   int damage;
   float range;
   sf::Sprite towerSprite;

public:
   Tower(int d, float r);
   void setPosition(sf::Vector2f pos);
   int getDamage();
   int getRange();
   void draw(sf::RenderTarget& target);

};

#endif