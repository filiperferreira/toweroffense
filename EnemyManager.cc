#include "EnemyManager.hh"

EnemyManager::EnemyManager(LevelParser level){
	towerPos = level.getTowerPositions();
	for(int i = 0; i < towerPos.size(); i++){
		Tower t(1, 70.0);
		t.setPosition(towerPos[i] + sf::Vector2f(15,15));
		towers.push_back(t);
	}
}   

void EnemyManager::drawTowers(sf::RenderTarget& target){
	for(int i = 0; i < towers.size(); i++){
		towers[i].draw(target);
	}
}

void EnemyManager::pushMinion(Minion m){
	minions.push_back(m);
}

void EnemyManager::drawMinions(sf::RenderTarget& target){
	for(int i = 0; i < minions.size(); i++){
		minions[i].draw(target);
	}
}

float EnemyManager::distance(Tower t, Minion m){
    float x1, x2, y1, y2;
    x1 = t.getPosition().x;
    y1 = t.getPosition().y;
    x2 = m.getPosition().x;
    y2 = m.getPosition().y;
    return sqrt(pow((x1-x2),2) + pow((y1-y2),2));
}

void EnemyManager::towerAttack(Tower& t, Minion& m){
	if (t.canAttack())
		if(distance(t, m) <= t.getRange()){
		    m.damage(t.getDamage());
		    t.resetCooldown();
		}
}

void EnemyManager::attack(vector<Minion>& minions){
	for(int j = 0 ; j < towers.size(); j++){
        for(int i = 0 ; i < minions.size(); i++){
            towerAttack(towers[j], minions[i]);
            if (!minions[i].isAlive()){
            	minions.erase(minions.begin()+i);
            }
        }
    }
}