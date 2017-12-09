#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace LevelConstants {
  const int SPRITE_SIZE = 64;

  const string GRASS_TILE  = "resources/textures/nonpath.png";
  const string GROUND_TILE = "resources/textures/path.png";

  const char GRASS         = 'X';
  const char PATH          = 'O';
  const char SPAWN         = 'S';
  const char TOWER         = 'T';
  const char GUARDIAN      = 'W';

  const int LEVEL1_ID  = 1;
  const int LEVEL2_ID  = 2;
  const int LEVEL3_ID  = 3;

  const string NEXT_LEVEL_SOUND = "resources/sounds/nextlevel.ogg";
}

namespace Level1 {
  const string LEVEL_FILE = "resources/maps/level1.bin";
  const string MUSIC_FILE = "resources/sounds/level1.ogg";
  const string UNLOCKABLE = "resources/textures/level1_unlockable.png";
}

namespace Level2 {
  const string LEVEL_FILE = "resources/maps/level2.bin";
  const string MUSIC_FILE = "resources/sounds/level2.ogg";
  const string UNLOCKABLE = "resources/textures/level2_unlockable.png";
}

namespace Level3 {
  const string LEVEL_FILE = "resources/maps/level3.bin";
  const string MUSIC_FILE = "resources/sounds/level3.ogg";
  const string UNLOCKABLE = "resources/textures/endgame.png";
}

namespace MinionConstants{  

  const string WATER_MINION   = "resources/textures/blue.png";
  const string EARTH_MINION   = "resources/textures/green.png";
  const string FIRE_MINION    = "resources/textures/yellow.png";
  const string WIND_MINION    = "resources/textures/purple.png";

}

namespace MenuConstants {  

  const string MENU     = "resources/textures/menu.png";
  const string SLIMIE   = "resources/textures/slimie_card.png";
  const string SKULLIE  = "resources/textures/skullie_card.png";
  const string GHOSTIE  = "resources/textures/ghostie_card.png";
  const string BATSIE   = "resources/textures/batsie_card.png";

}
#endif