#include "Player.hh"
#include <sstream>

using namespace MenuConstants;

template <typename T>
std::string NumberToString ( T Number ) {
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

Player::Player(int levelid, LevelParser lp) {
	menu.setTexture(*(TextureManager::getTexture(MENU)));
	slimie.setTexture(*(TextureManager::getTexture(SLIMIE)));
	slimie.setPosition(70,25);
	skullie.setTexture(*(TextureManager::getTexture(SKULLIE)));
	skullie.setPosition(slimie.getPosition() + sf::Vector2f(140, 0));
	switch(levelid){
        case LevelConstants::LEVEL3_ID:
        	ghostie.setTexture(*(TextureManager::getTexture(GHOSTIE)));
			ghostie.setPosition(skullie.getPosition() + sf::Vector2f(140, 0));
        	batsie.setTexture(*(TextureManager::getTexture(BATSIE)));
			batsie.setPosition(ghostie.getPosition() + sf::Vector2f(140, 0));
		break;
        case LevelConstants::LEVEL2_ID:
        	ghostie.setTexture(*(TextureManager::getTexture(GHOSTIE)));
			ghostie.setPosition(skullie.getPosition() + sf::Vector2f(140, 0));
        break;
    }
    if (!MyFont.loadFromFile(TOY_FONT)) {
        printf("error");
    }
    wallet = lp.getWallet();
    walletText.setFont(MyFont);
    walletText.setString("$" + NumberToString(wallet));
    walletText.setCharacterSize(100);
    walletText.setFillColor(sf::Color(224, 150, 168));
    walletText.setPosition(sf::Vector2f(820, 25));
}

void Player::draw(sf::RenderTarget& target){
    walletText.setString("$" + NumberToString(wallet));
    target.draw(menu);
    target.draw(slimie);
    target.draw(skullie);
    target.draw(ghostie);
    target.draw(batsie);
    target.draw(walletText);
}

bool Player::buy(Minion m){
	if (wallet >= m.getPrice()){
		wallet -= m.getPrice();
		return true;
	}
	return false;
}

int Player::getWallet(){
    return wallet;
}

Player::~Player() {

}