#include <iostream>
#include <String>
#include "Minion.hh"
#include "LevelParser.hh"
#include "Constants.hh"

#include <SFML/Graphics.hpp>


class NextLevel : public ScreenManager {
private:
	LevelParser thisLevel;
	sf::View view;
	sf::Clock clock;
    string unlockable;
    int nextscreen;

public:
	NextLevel(int level);
	virtual int Run(sf::RenderWindow &App);
};

NextLevel::NextLevel(int levelid) {
    switch(levelid){
        case LevelConstants::LEVEL2_ID:
            unlockable = Level1::UNLOCKABLE;
            nextscreen = 3;
        break;
        case LevelConstants::LEVEL3_ID:
            unlockable = Level2::UNLOCKABLE;
            nextscreen = 5;
        break;
    }
}

int NextLevel::Run(sf::RenderWindow &window) {
    sf::Music music;
    if (!music.openFromFile(LevelConstants::NEXT_LEVEL_SOUND)) {
        return -1;
    }
    music.setVolume(50);
    music.play();  

    TextureManager::loadTexture(unlockable, unlockable);
    sf::Sprite background(*(TextureManager::getTexture(unlockable)));

    bool running = true;
    sf::Event Event;
    while(running){
        while(window.pollEvent(Event)){
            if (Event.type == sf::Event::Closed) {
                return (-1);
            }
            //Key pressed
            if (Event.type == sf::Event::KeyPressed){
                if (sf::Keyboard::Return)
                    return (nextscreen);
            }
        }
        //Clearing screen
        window.clear();
        //Drawing
        window.draw(background);
        window.display();
    }
    //Never reaching this point normally, but just in case, exit the application
    return -1;
}