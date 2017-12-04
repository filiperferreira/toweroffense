#ifndef MINION_H
#define MINION_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <string>
#include <vector>
#include <deque>
#include <string>

using namespace std;

class Minion {
   float speed, health, initialHealth;
   sf::Sprite minionSprite;
   deque<sf::Vector2f> movements;
   sf::Vector2f currPosition;
   sf::RectangleShape healthBar;

   Minion();
public:
   ~Minion();
   Minion(string minionType, sf::Vector2f initPos, deque<sf::Vector2f> initMovements);
   void draw(sf::RenderTarget& target);
   void move(sf::Time timeElapsed);
   void updateHealthBar(float hlt);
   void damage(float dam);

   //getters and setters
   bool isAlive();
   void setPosition(sf::Vector2f position);
   sf::Vector2f getPosition();
   void setSpeed(float spd);
   float getSpeed();
   void setHealth(float hlt);
   float getHealth();
};

#endif