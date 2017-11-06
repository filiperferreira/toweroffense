#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class levelMap {
    int sizeX, sizeY;
    map<int, string> levelFiles;
    map<char, sf::Texture> textureMap;
    vector<sf::Texture> texture;
    vector<vector<sf::Sprite>> tile;

    public:
    levelMap() {
        levelFiles[1] = "resources/maps/level1.bin";

        texture = vector<sf::Texture>(2);
        texture[0].loadFromFile("resources/textures/land.png");
        texture[1].loadFromFile("resources/textures/water.png");

        textureMap['X'] = texture[0];
        textureMap['O'] = texture[1];
    }

    void loadLevel(int levelId) {
        ifstream mapFile;

        mapFile.open(levelFiles[levelId]);
        if (mapFile.is_open()) {
            mapFile >> sizeX >> sizeY;
            tile = vector<vector<sf::Sprite>>(sizeX);

            for (int i = 0; i < sizeX; i++) {
                tile[i] = vector<sf::Sprite>(sizeY);
                for (int j = 0; j < sizeY; j++) {
                    char curTile;

                    mapFile >> curTile;

                    tile[i][j].setTexture(textureMap[curTile]);
                    tile[i][j].setPosition(64*j, 64*i);
                }
            }

            mapFile.close();
        }
        else {
            cerr << "Error opening level data." << endl;
        }
    }

    int mapSizeX() {
        return sizeX;
    }

    int mapSizeY() {
        return sizeY;
    }

    sf::Sprite getTile(int x, int y) {
        return tile[x][y];
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Tower Offense");

    sf::View view(sf::Vector2f(512, 512), sf::Vector2f(768, 672));
    view.setViewport(sf::FloatRect(0.25, 0, 0.75, 0.875));

    levelMap thisLevel;
    thisLevel.loadLevel(1);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        window.setView(view);
        for (int i = 0; i < thisLevel.mapSizeX(); i++) {
            for (int j = 0; j < thisLevel.mapSizeY(); j++) {
                window.draw(thisLevel.getTile(i, j));
            }
        }

        window.display();
    }

    return 0;
}
