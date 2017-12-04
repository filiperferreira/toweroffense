#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;

class Tower {
   int damage;
   float range;

public:
   Tower(int d, float r);

};

#endif