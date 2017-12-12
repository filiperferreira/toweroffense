#include <iostream>
#include <string>
#include "Minion.hh"
#include "LevelParser.hh"
#include "Player.hh"
#include "Constants.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace MinionConstants;

class Level : public ScreenManager {
private:
	LevelParser thisLevel;
    Player* player;
	vector<Minion> minions;
    int minionCount = 0;
	sf::Clock clock;
    string levelfile;
    string musicfile;
    int nextscreen; 
    int permission = 0;

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
            permission = 1;
        break;
        case LevelConstants::LEVEL2_ID:
            levelfile = Level2::LEVEL_FILE;
            musicfile = Level2::MUSIC_FILE;
            nextscreen = 4;
            permission = 2;
        break;
        case LevelConstants::LEVEL3_ID:
            levelfile = Level3::LEVEL_FILE;
            musicfile = Level3::MUSIC_FILE;
            nextscreen = 6;
            permission = 3;
        break;
    }
    player = new Player(levelid);
}

bool eop(sf::Vector2f a, sf::Vector2f b){
    return a.x >= b.x && a.y >= b.y;
}

bool gameOver(Player* player, vector<Minion> minions){
    return player->getWallet() < 50 && minions.size() == 0;
}

int Level::Run(sf::RenderWindow &window) {
    thisLevel.loadLevel(levelfile);
    player->setLevel(thisLevel);

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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    if (permission >= 1){
                        Minion m(SLIMIE_MINION, thisLevel);
                        m.setSpeed(10);
                        m.setHealth(200);
                        m.setPrice(50);
                        if (player->buy(m)) minions.push_back(m);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    if (permission >= 1){
                        Minion m(SKULLIE_MINION, thisLevel);
                        m.setSpeed(20);
                        m.setHealth(50);
                        m.setPrice(100);
                        if (player->buy(m)) minions.push_back(m);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    if (permission >= 2){
                        Minion m(GHOSTIE_MINION, thisLevel);
                        m.setSpeed(15);
                        m.setHealth(80);
                        m.setPrice(200);
                        if (player->buy(m)) minions.push_back(m);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    if (permission >= 3){
                        Minion m(BASTIE_MINION, thisLevel);
                        m.setSpeed(30);
                        m.setHealth(50);
                        m.setPrice(500);
                        if (player->buy(m)) minions.push_back(m);
                    }
                }
            }
        }

        window.clear();

        for (int i = 0; i < thisLevel.mapSizeX(); i++) {
            for (int j = 0; j < thisLevel.mapSizeY(); j++) {
                window.draw(thisLevel.getTile(i, j));
            }
        }
        for (int i = 0; i < minions.size(); i++) {
            minions[i].move(timeElapsed);
            minions[i].draw(window);
            if (eop(minions[i].getPosition(), thisLevel.getEndOfPath())){
                minionCount++;
                minions.erase(minions.begin()+i);
            }
        }
        
        if (minionCount > 4){
            return nextscreen;
        }

        if (gameOver(player, minions)){
            return 0;
        }
        player->draw(window);
        window.display();
    }
	//Never reaching this point normally, but just in case, exit the application
	return -1;
}