#pragma once

#include "entity.hpp"
#include "entity_manager.hpp"
#include <SFML/Graphics.hpp>

struct WindowConfig {int FR, FS, BR, BG, BB; unsigned int W, H;};
struct BulletConfig {int SR, CR, OT, V, L, FR, FG, FB, OR, OG, OB; float S;};
struct EnemyConfig  {int SR, CR, OT, L, SI, VMIN, VMAX, OR, OG, OB; float SMIN, SMAX;};
struct TowerConfig  {int SR, CR, OT, V, FR, FG, FB, OR, OG, OB;};

class Game
{
	bool 		            m_paused = false;
	int  		            m_currentFrame = 0;
	int  		            m_lastEnemySpawnTime = 0;
	int                     m_score = 0;
	
	sf::Clock				m_deltaClock;
	sf::Font				m_font;
	sf::Text				m_text;
	sf::RenderWindow		m_window;
	
	WindowConfig    		m_windowConfig;
	BulletConfig            m_bulletConfig;
	EnemyConfig             m_enemyConfig;
	EntityManager           m_entities;
	TowerConfig				m_towerConfig;
	
	std::shared_ptr<Entity> tower();
	
	void init(const std::string & config);
	void readConfigFile(const std::string & config);
	void setPaused(bool & paused);
	
	void sCollision();
	void sEnemySpawner();
	void sLifespan();
	void sMovement();
	void sRender();
	void sUserInput();
	
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f & target);
	void spawnEnemy();
	void spawnTower(const Vec2f & mousePos);
	void spawnTower();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	
public:	
	Game(const std::string & config);
	void run();
};