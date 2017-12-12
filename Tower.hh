#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "Constants.hh"

using namespace std;

class Tower {
   int damage;
   int passedTime;
   float range;
   sf::Sprite towerSprite;
   sf::Clock attackClock;

public:
   Tower(int d, float r);
   void setPosition(sf::Vector2f pos);
   sf::Vector2f getPosition();
   int getDamage();
   int getRange();
   void draw(sf::RenderTarget& target);
   int getClock();
   void resetCooldown();
   bool canAttack();
};

#endif