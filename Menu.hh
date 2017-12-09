#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>

class Menu : public ScreenManager {
private:
    bool playing;
    int nextscreen = 1; 
    int exitscreen = -1;

public:
	Menu(void);
	virtual int Run(sf::RenderWindow &App);
};

Menu::Menu(void) {
    playing       = false;
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
    play.setTextureRect(sf::IntRect(0, 0, 285, 149));
    play.setPosition(sf::Vector2f(180, 550));
    //play.setScale(sf::Vector2f(4, 4));

    exit.setTexture(*(TextureManager::getTexture("buttons")));
    exit.setTextureRect(sf::IntRect(282, 0, 285, 149));
    exit.setPosition(sf::Vector2f(540, 550));
    //exit.setScale(sf::Vector2f(4, 4));

    sf::Sprite background(*(TextureManager::getTexture("title")));

    sf::Music music;
    if (!music.openFromFile("resources/sounds/theme.ogg")) {
        return exitscreen;
    }
    music.setVolume(50);         // reduce the volume
    music.setLoop(true);         // make it loop
    music.play();

    bool running = true;

    while(running){
        while(window.pollEvent(Event)){
            if (Event.type == sf::Event::Closed) {
                return (exitscreen);
            }
            //Key pressed
            if (Event.type == sf::Event::KeyPressed){
                switch (Event.key.code){
                case sf::Keyboard::Left:
                    menu = 0;
                    play.setTexture(*(TextureManager::getTexture("highlights")));
                    exit.setTexture(*(TextureManager::getTexture("buttons")));
                    break;
                case sf::Keyboard::Right:
                    exit.setTexture(*(TextureManager::getTexture("highlights")));
                    play.setTexture(*(TextureManager::getTexture("buttons")));
                    menu = 1;
                    break;
                case sf::Keyboard::Return:
                    if (menu == 0){
                        //Let's get play !
                        playing = true;
                        return (nextscreen);
                    } else {
                        //Let's get work...
                        return (exitscreen);
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
	return exitscreen;
}