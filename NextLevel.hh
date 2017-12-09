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
        case LevelConstants::LEVEL1_ID:
            unlockable = Level1::UNLOCKABLE;
            nextscreen = 3;
        break;
        case LevelConstants::LEVEL2_ID:
            unlockable = Level2::UNLOCKABLE;
            nextscreen = 5;
        break;
        case LevelConstants::LEVEL3_ID:
            unlockable = Level3::UNLOCKABLE;
            nextscreen = -1;
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
    view.setCenter(sf::Vector2f(512, 384));
    view.setSize(sf::Vector2f(1024, 768));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));

    bool running = true;
    sf::Event Event;
    while(running){
        while(window.pollEvent(Event)){
            if (Event.type == sf::Event::Closed) {
                return (-1);
            }
            if (Event.type == sf::Event::KeyPressed){
                return (nextscreen);
            }
        }
        window.clear();
        window.setView(view);
        window.draw(background);
        window.display();
    }
    return -1;
}