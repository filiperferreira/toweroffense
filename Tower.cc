#include "Tower.hh"

Tower::Tower(int d, float r): damage(d), range(r) {
	towerSprite.setTexture(*(TextureManager::getTexture(TowerConstants::DESTROYER)));
}

void Tower::setPosition(sf::Vector2f pos) {
	towerSprite.setPosition(pos);
}

int Tower::getDamage() {
	return damage;
}

int Tower::getRange() {
	return range;
}

void Tower::draw(sf::RenderTarget& target){
    target.draw(towerSprite);
}