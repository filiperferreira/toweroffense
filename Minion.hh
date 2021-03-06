#ifndef MINION_H
#define MINION_H

#include <SFML/Graphics.hpp>
#include "TextureManager.hh"
#include "LevelParser.hh"
#include <string>
#include <vector>
#include <deque>

using namespace std;

class Minion {
   float speed, health, initialHealth;
   int price;
   sf::Sprite minionSprite;
   deque<pair<sf::Vector2f,sf::Vector2f>> movements;
   sf::RectangleShape healthBar;

   Minion();
public:
   ~Minion();
   Minion(string minionType, LevelParser lp);
   void draw(sf::RenderTarget& target);
   void move(sf::Time timeElapsed);
   void updateHealthBar(float hlt);
   void damage(float dam);
   bool isAlive();
   sf::Vector2f getPosition();
   void setSpeed(float spd);
   float getSpeed();
   void setHealth(float hlt);
   float getHealth();
   sf::Vector2f normalize(sf::Vector2f& source);
   bool moveTo(sf::Time timeElapsed, sf::Vector2f from, sf::Vector2f to);
   int getPrice();
   void setPrice(int price);
};

#endif