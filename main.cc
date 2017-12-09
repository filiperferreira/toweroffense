#include <SFML/Graphics.hpp>
#include <vector>

#include "TextureManager.hh"
#include "Minion.hh"
#include "Tower.hh"
#include "LevelParser.hh"
#include "Constants.hh"
#include "Screens.hh"

using namespace std;
using namespace MinionConstants;

void loadTextures(){
    // needs to name the texture and then call getTexture by its name
    TextureManager::loadTexture(WATER_MINION, WATER_MINION);
    TextureManager::loadTexture(EARTH_MINION, EARTH_MINION);
    TextureManager::loadTexture(FIRE_MINION, FIRE_MINION);
    TextureManager::loadTexture(WIND_MINION, WIND_MINION);
}

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "The Saga of Urkiorus");

    loadTextures();

    std::vector<ScreenManager*> screens;
    int screen = 0;
    
    Menu s0;
    screens.push_back(&s0);

    Level s1(LevelConstants::LEVEL1_ID);
    screens.push_back(&s1);

    NextLevel s12(LevelConstants::LEVEL1_ID);
    screens.push_back(&s12);
    
    Level s2(LevelConstants::LEVEL2_ID);
    screens.push_back(&s2);

    NextLevel s23(LevelConstants::LEVEL2_ID);
    screens.push_back(&s23);
    
    Level s3(LevelConstants::LEVEL3_ID);
    screens.push_back(&s3);
    
    while (screen >= 0) {
        screen = screens[screen]->Run(window);
    }

    return 0;
}
