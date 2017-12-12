#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <SFML/Graphics.hpp>
#include "Tower.hh"
#include "Minion.hh"
#include "LevelParser.hh"
#include <vector>

using namespace std;

class EnemyManager {
   vector<Tower> towers;
   vector<Minion> minions;
   vector<sf::Vector2f> towerPos;

public:
   EnemyManager(LevelParser level);
   void drawTowers(sf::RenderTarget& target);
   void drawMinions(sf::RenderTarget& target);
   void pushMinion(Minion m);
   float distance(Tower t, Minion m);
   void towerAttack(Tower& t, Minion& m);
   void attack(vector<Minion>& minions);
};

#endif