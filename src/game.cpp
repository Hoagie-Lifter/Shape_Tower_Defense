#include "game.h"
#include <iostream>
#include <fstream>

Game::Game(const std::string & config)
	: m_text(m_font, "Default", 24)
{
	init(config);	
}

void Game::init(const std::string & config)
{
	readConfigFile(config);
	
	m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2");
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);
	
	spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
	return m_entities.getEntities("player").back();
}

void Game::run()
{
	// TODO: add pause functionality
	
	while (true)
	{
		m_entities.update();
		
		//sUserInput();
		//sEnemySpawner();
		//sMovement();
		//sCollision();
		//sRender();
		
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	// TODO
}

void Game::spawnPlayer()
{
	// TODO: finish adding all player values
	
	auto e = m_entities.addEntity("player");
	
	e -> add<CInput>();
	e -> add<CShape>();
	e -> add<CTransform>();
}

void Game::readConfigFile(const std::string & config)
{
	std::ifstream fin(config);
	std::string token;
	while (fin >> token)
	{
		if (token == "Window:")
		{
			// WindowConfig {int FR, FS, BR, BG, BB; unsigned int W, H;};
			
			fin >> m_windowConfig.FR;
			fin >> m_windowConfig.FS;
			fin >> m_windowConfig.BR;
			fin >> m_windowConfig.BG;
			fin >> m_windowConfig.BB;
			fin >> m_windowConfig.W;
			fin >> m_windowConfig.H;
		}
		
		else if (token == "Bullet:")
		{
			// BulletConfig {int SR, CR, OT, V, FR, FG, FB, OR, OG, OB; float S;};
			
			fin >> m_bulletConfig.SR;
			fin >> m_bulletConfig.CR;
			fin >> m_bulletConfig.OT;
			fin >> m_bulletConfig.V;
			fin >> m_bulletConfig.FR;
			fin >> m_bulletConfig.FG;
			fin >> m_bulletConfig.FB;
			fin >> m_bulletConfig.OR;
			fin >> m_bulletConfig.OG;
			fin >> m_bulletConfig.OB;
			fin >> m_bulletConfig.S;
		}
		
		else if (token == "Enemy:")
		{
			// EnemyConfig  {int SR, CR, OT, L, SI, VMIN, VMAX, OR, OG, OB; float SMIN, SMAX;};
			
			fin >> m_enemyConfig.SR;
			fin >> m_enemyConfig.CR;
			fin >> m_enemyConfig.OT;
			fin >> m_enemyConfig.L;
			fin >> m_enemyConfig.SI;
			fin >> m_enemyConfig.VMIN;
			fin >> m_enemyConfig.VMAX;
			fin >> m_enemyConfig.OR;
			fin >> m_enemyConfig.OG;
			fin >> m_enemyConfig.OB;
			fin >> m_enemyConfig.SMIN;
			fin >> m_enemyConfig.SMAX;
		}
		
		else if (token == "Player:")
		{
			// PlayerConfig {int SR, CR, OT, V, FR, FG, FB, OR, OG, OB; float S;};
			
			fin >> m_playerConfig.SR;
			fin >> m_playerConfig.CR;
			fin >> m_playerConfig.OT;
			fin >> m_playerConfig.V;
			fin >> m_playerConfig.FR;
			fin >> m_playerConfig.FG;
			fin >> m_playerConfig.FB;
			fin >> m_playerConfig.OR;
			fin >> m_playerConfig.OG;
			fin >> m_playerConfig.OB;
			fin >> m_playerConfig.S;
		}
	}
}