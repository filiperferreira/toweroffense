#include "Tower.hh"

Tower::Tower(int d, float r): damage(d), range(r) {
	attackClock.restart();
	towerSprite.setTexture(*(TextureManager::getTexture(TowerConstants::DESTROYER)));
}

void Tower::setPosition(sf::Vector2f pos) {
	towerSprite.setPosition(pos);
}

sf::Vector2f Tower::getPosition() {
	return towerSprite.getPosition();
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

int Tower::getClock(){
    return attackClock.getElapsedTime().asMilliseconds();
}

void Tower::resetCooldown(){
	attackClock.restart();
}

bool Tower::canAttack(){
	passedTime += getClock();
	if (passedTime > 100){
		passedTime = 0;
		resetCooldown();
		return true;
	}
	return false;
}