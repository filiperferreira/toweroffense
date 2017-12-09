#include <iostream>
#include <string>
#include "Minion.hh"
#include "LevelParser.hh"
#include "Constants.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace MinionConstants;

class Level : public ScreenManager {
private:
	LevelParser thisLevel;
	sf::View view;
	vector<Minion> minions;
    int minionCount = 0;
	sf::Clock clock;
    string levelfile;
    string musicfile;
    int nextscreen; 

public:
	Level(int level);
	virtual int Run(sf::RenderWindow &App);
};

Level::Level(int levelid) {
    minionCount = 0;
    switch(levelid){
        case LevelConstants::LEVEL1_ID:
            levelfile  = Level1::LEVEL_FILE;
            musicfile  = Level1::MUSIC_FILE;
            nextscreen = 2;
        break;
        case LevelConstants::LEVEL2_ID:
            levelfile = Level2::LEVEL_FILE;
            musicfile = Level2::MUSIC_FILE;
            nextscreen = 4;
        break;
        case LevelConstants::LEVEL3_ID:
            levelfile = Level3::LEVEL_FILE;
            musicfile = Level3::MUSIC_FILE;
            nextscreen = 6;
        break;
    }
}

bool eop(sf::Vector2f a, sf::Vector2f b){
    return a.x >= b.x and a.y >= b.y;
}

int Level::Run(sf::RenderWindow &window) {
    thisLevel.loadLevel(levelfile);
    view.setCenter(sf::Vector2f(512, 384));
    view.setSize(sf::Vector2f(1024, 768));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));

    sf::Music music;
    if (!music.openFromFile(musicfile)) {
        return -1;
    }
    music.setVolume(50);         // reduce the volume
    music.setLoop(true);         // make it loop
    music.play();

    while (window.isOpen()) {
        sf::Event event;
        sf::Time timeElapsed;

        timeElapsed = clock.restart();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (view.getCenter().x - 512 > 0) {
                        view.move(-64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (view.getCenter().x + 512 < thisLevel.mapSizeX() * 64) {
                        view.move(64, 0);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    if (view.getCenter().y - 384 > 0) {
                        view.move(0, -64);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (view.getCenter().y + 384 < thisLevel.mapSizeY() * 64) {
                        view.move(0, 64);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    Minion m(WATER_MINION, thisLevel);
                    m.setSpeed(10);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    Minion m(EARTH_MINION, thisLevel);
                    m.setSpeed(20);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    Minion m(WIND_MINION, thisLevel);
                    m.setSpeed(30);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    Minion m(FIRE_MINION, thisLevel);
                    m.setSpeed(15);
                    m.setHealth(100);

                    minions.push_back(m);
                }
            }
        }

        window.clear();

        window.setView(view);
        for (int i = 0; i < thisLevel.mapSizeX(); i++) {
            for (int j = 0; j < thisLevel.mapSizeY(); j++) {
                window.draw(thisLevel.getTile(i, j));
            }
        }
        for (int i = 0; i < minions.size(); i++) {
            minions[i].move(timeElapsed);
            minions[i].draw(window);
            //printf("%f %f, %f %f\n", minions[0].getPosition().x, minions[0].getPosition().y, thisLevel.getEndOfPath().x, thisLevel.getEndOfPath().y);
            if (eop(minions[i].getPosition(), thisLevel.getEndOfPath())){
                //printf("%f %f", thisLevel.getEndOfPath().x, thisLevel.getEndOfPath().y);
                minionCount++;
                minions.erase(minions.begin()+i);
            }
        }
        if (minionCount > 4){
            return nextscreen;
        }
        window.display();
    }
	//Never reaching this point normally, but just in case, exit the application
	return -1;
}