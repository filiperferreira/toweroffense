#include <SFML/Graphics.hpp>


class Menu : public ScreenManager {
private:
    bool playing;

public:
	Menu(void);
	virtual int Run(sf::RenderWindow &App);
};

Menu::Menu(void) {
    playing = false;
}

int Menu::Run(sf::RenderWindow &window) {
    sf::Sprite play;
    sf::Sprite exit;
    sf::Event Event;
    int menu = -1;
    TextureManager::loadTexture("buttons",    "resources/textures/buttons.png");
    TextureManager::loadTexture("highlights", "resources/textures/buttonshighlights.png");
    TextureManager::loadTexture("title",      "resources/textures/title.png");

    play.setTexture(*(TextureManager::getTexture("buttons")));
    play.setTextureRect(sf::IntRect(0, 0, 40, 21));
    play.setPosition(sf::Vector2f(450, 300));
    play.setScale(sf::Vector2f(4, 4));

    exit.setTexture(*(TextureManager::getTexture("buttons")));
    exit.setTextureRect(sf::IntRect(41, 0, 40, 21));
    exit.setPosition(sf::Vector2f(450, 400));
    exit.setScale(sf::Vector2f(4, 4));

    sf::Sprite background(*(TextureManager::getTexture("title")));

    bool running = true;

    while(running){
        while(window.pollEvent(Event)){
            if (Event.type == sf::Event::Closed) {
                return (-1);
            }
            //Key pressed
            if (Event.type == sf::Event::KeyPressed){
                switch (Event.key.code){
                case sf::Keyboard::Up:
                    menu = 0;
                    play.setTexture(*(TextureManager::getTexture("highlights")));
                    exit.setTexture(*(TextureManager::getTexture("buttons")));
                    break;
                case sf::Keyboard::Down:
                    exit.setTexture(*(TextureManager::getTexture("highlights")));
                    play.setTexture(*(TextureManager::getTexture("buttons")));
                    menu = 1;
                    break;
                case sf::Keyboard::Return:
                    if (menu == 0){
                        //Let's get play !
                        playing = true;
                        return (1);
                    } else {
                        //Let's get work...
                        return (-1);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        //Clearing screen
        window.clear();
        //Drawing
        window.draw(background);
        window.draw(play);
        window.draw(exit);
        window.display();
    }
	//Never reaching this point normally, but just in case, exit the application
	return -1;
}