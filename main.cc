#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "TextureManager.hh"
#include "Minion.hh"
#include "Tower.hh"
#include "LevelParser.hh"
#include "Constants.hh"
#include "Screens.hh"

using namespace std;
using namespace MinionConstants;

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "The Saga of Urkiorus");

    // needs to name the texture and then call getTexture by its name
    TextureManager::loadTexture(WATER_MINION, WATER_MINION);
    TextureManager::loadTexture(EARTH_MINION, EARTH_MINION);
    TextureManager::loadTexture(FIRE_MINION, FIRE_MINION);
    TextureManager::loadTexture(WIND_MINION, WIND_MINION);

    std::vector<ScreenManager*> screens;
    int screen = 0;
    
    Level s0("resources/maps/level4.bin");
    screens.push_back(&s0);
    Level s1("resources/maps/level3.bin");
    screens.push_back(&s1);

    while (screen >= 0) {
        screen = screens[screen]->Run(window);
    }

    return 0;
}
