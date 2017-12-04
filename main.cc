#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "TextureManager.h"
#include "Minion.h"
#include "Tower.h"
#include "LevelParser.h"

using namespace std;

int main() {
    sf::Clock timer;
    float minionTime = timer.restart().asSeconds();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Tower Offense");
    sf::Clock clock;

    sf::View view(sf::Vector2f(512, 384), sf::Vector2f(1024, 768));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    /*sf::View menu(sf::Vector2f(), sf::Vector2f(256, 672));
    menu.setViewport(sf::FloatRect(0, 0, 0.25, 0.875));*/

    LevelParser thisLevel;
    thisLevel.loadLevel("resources/maps/level4.bin");
    
    // needs to name the texture and then call getTexture by its name
    TextureManager::loadTexture("blue", "resources/textures/blue.png");
    TextureManager::loadTexture("green", "resources/textures/green.png");
    TextureManager::loadTexture("purple", "resources/textures/purple.png");
    TextureManager::loadTexture("yellow", "resources/textures/yellow.png");

    vector<Minion> minions;

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
                    Minion m("blue", sf::Vector2f(thisLevel.getSpawnPosX(), thisLevel.getSpawnPosY()), thisLevel.getPath());
                    m.setSpeed(10);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    Minion m("green", sf::Vector2f(thisLevel.getSpawnPosX(), thisLevel.getSpawnPosY()), thisLevel.getPath());
                    m.setSpeed(10);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    Minion m("purple",  sf::Vector2f(thisLevel.getSpawnPosX(), thisLevel.getSpawnPosY()), thisLevel.getPath());
                    m.setSpeed(10);
                    m.setHealth(100);

                    minions.push_back(m);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    Minion m("yellow",  sf::Vector2f(thisLevel.getSpawnPosX(), thisLevel.getSpawnPosY()), thisLevel.getPath());
                    m.setSpeed(10);
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
        }
        window.display();
    }

    return 0;
}
