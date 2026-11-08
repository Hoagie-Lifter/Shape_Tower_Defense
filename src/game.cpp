#include "game.h"
#include <iostream>

Game::Game(const std::string & config)
	: m_text(m_font, "Default", 24)
{
	init(config);	
}

void Game::init(const std::string & path)
{
	// TODO: read in config file
	
	m_window.create(sf::VideoMode({1280, 720}), "Assignment 2");
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60)
	
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
		m_entities.update()
		
		sUserInput();
		sEnemySpawner();
		sMovement();
		sCollision();
		sRender();
		
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
	
	e -> add<CInput>()
	e -> add<CShape>()
	e -> add<CTransform>()
}

