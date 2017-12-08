#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace LevelConstants {
  const int SPRITE_SIZE = 64;

  const string GRASS_TILE  = "resources/textures/grass.png";
  const string GROUND_TILE = "resources/textures/ground.png";

  const char GRASS         = 'X';
  const char PATH          = 'O';
  const char SPAWN         = 'S';
  const char TOWER         = 'T';
}

namespace MinionConstants{  

  const string WATER_MINION   = "resources/textures/blue.png";
  const string EARTH_MINION   = "resources/textures/green.png";
  const string FIRE_MINION    = "resources/textures/yellow.png";
  const string WIND_MINION    = "resources/textures/purple.png";
}
#endif