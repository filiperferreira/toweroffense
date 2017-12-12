#include "EnemyManager.hh"

EnemyManager::EnemyManager(LevelParser level){
	towerPos = level.getTowerPositions();
	for(int i = 0; i < towerPos.size(); i++){
		Tower t(10, 10);
		t.setPosition(towerPos[i] + sf::Vector2f(15,15));
		towers.push_back(t);
	}
}   

void EnemyManager::drawTowers(sf::RenderTarget& target){
	for(int i = 0; i < towers.size(); i++){
		towers[i].draw(target);
	}
}