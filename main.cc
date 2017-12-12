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
    TextureManager::loadTexture(SLIMIE_MINION, SLIMIE_MINION);
    TextureManager::loadTexture(SKULLIE_MINION, SKULLIE_MINION);
    TextureManager::loadTexture(GHOSTIE_MINION, GHOSTIE_MINION);
    TextureManager::loadTexture(BASTIE_MINION, BASTIE_MINION);

    TextureManager::loadTexture(MenuConstants::MENU, MenuConstants::MENU);
    TextureManager::loadTexture(MenuConstants::SLIMIE, MenuConstants::SLIMIE);
    TextureManager::loadTexture(MenuConstants::SKULLIE, MenuConstants::SKULLIE);
    TextureManager::loadTexture(MenuConstants::GHOSTIE, MenuConstants::GHOSTIE);
    TextureManager::loadTexture(MenuConstants::BATSIE, MenuConstants::BATSIE);
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

    NextLevel send(LevelConstants::LEVEL3_ID);
    screens.push_back(&send);
    
    while (screen >= 0) {
        screen = screens[screen]->Run(window);
    }

    return 0;
}
