#pragma once

#include "entity.hpp"
#include "entity_manager.hpp"
#include <SFML/Graphics.hpp>

struct BulletConfig {int shapeRadius}
struct EnemyConfig  {}
struct PlayerConfig {}

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
	
	BulletConfig            m_bulletConfig;
	EnemyConfig             m_enemyConfig;
	EntityManager           m_entities;
	PlayerConfig			m_playerConfig;
	
	std::shared_ptr<Entity> player();
	
	void init(const std::string & config);
	void setPaused(bool paused);
	
	void sCollision();
	void sEnemySpawner();
	void sLifespan();
	void sMovement();
	void sRender();
	void sUserInput();
	
	void spawnBullet();
	void spawnEnemy(std::shared_ptr<Entity> entity, const Vec2f & mousePos);
	void spawnPlayer();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	
public:	
	Game(const std:string & config);
	void run();
};