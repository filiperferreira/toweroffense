#include <iostream>
#include <String>
#include "Minion.hh"
#include "LevelParser.hh"
#include "Constants.hh"

#include <SFML/Graphics.hpp>

using namespace MinionConstants;

class Level : public ScreenManager {
private:
	LevelParser thisLevel;
	sf::View view;
	vector<Minion> minions;
	sf::Clock clock;

public:
	Level(std::string level);
	virtual int Run(sf::RenderWindow &App);
};

Level::Level(std::string level) {
	thisLevel.loadLevel(level);
	view.setCenter(sf::Vector2f(512, 384));
	view.setSize(sf::Vector2f(1024, 768));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

int Level::Run(sf::RenderWindow &window) {
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
                    //return 1;//next screen
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
        }
        window.display();
    }
	//Never reaching this point normally, but just in case, exit the application
	return -1;
}