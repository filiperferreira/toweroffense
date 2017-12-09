#include "Player.hh"

using namespace MenuConstants;


Player::Player(int levelid) {
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
}

void Player::draw(sf::RenderTarget& target, sf::View view){
    target.draw(menu);
    target.draw(slimie);
    target.draw(skullie);
    target.draw(ghostie);
    target.draw(batsie);
    //updatePosition(view);
}

void Player::updatePosition(sf::View view){
    menu.setPosition(view.getCenter()-sf::Vector2f(512, 384));
    /*slimie.move(menu.getPosition());
    skullie.move(menu.getPosition());
    ghostie.move(menu.getPosition());
    batsie.move(menu.getPosition());*/
}

bool Player::buy(Minion m){
	if (wallet > m.getPrice()){
		wallet -= m.getPrice();
		return true;
	}
	return false;
}

Player::~Player() {

}