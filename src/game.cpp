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
		
		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
			m_currentFrame++;
		}

		sUserInput();
		sRender();
	}
}

void Game::setPaused(bool & paused)
{
	paused = !paused;
}

void Game::spawnPlayer()
{
	auto e = m_entities.addEntity("player");
	
	e -> add<CInput>();
	e -> add<CShape>(
	m_playerConfig.SR,
	m_playerConfig.V,
	sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
	sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
	m_playerConfig.OT);
	
	e -> add<CTransform>(Vec2f(200, 200), Vec2f(m_playerConfig.S, m_playerConfig.S), 0);
}

void Game::spawnEnemy()
{
	auto e = m_entities.addEntity("enemy");
	
	e -> add<CShape>(
	m_enemyConfig.SR,
	m_enemyConfig.VMAX,
	sf::Color(0, 0, 255),
	sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
	m_enemyConfig.OT);
	
	e -> add<CTransform>(Vec2f(200, 200), Vec2f(m_enemyConfig.SMIN, m_enemyConfig.SMIN), 0);
	e -> add<CLifespan>(100);
	
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	//TODO
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f & target)
{
	//TODO
	//Vec2f direction = target.distance(entityPos);
	//Vec2f velocity = direction * m_bulletConfig.S;
	
	auto e = m_entities.addEntity("bullet");
	
	auto & entityPos = entity -> get<CTransform>().pos;
	Vec2f direction = target - entityPos;
	direction.normalize();
	Vec2f velocity = direction * m_bulletConfig.S;
	
	e -> add<CShape>(
	m_bulletConfig.SR, 
	m_bulletConfig.V,
	sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
	sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
	m_bulletConfig.OT);
	
	e -> add<CTransform>(Vec2f(entityPos.x, entityPos.y), Vec2f(velocity.x, velocity.y), 0);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO
}

void Game::sLifespan()
{
	for (auto e : m_entities.getEntities())
	{
		if (e -> has<CLifespan>())
		{
			if (e -> get<CLifespan>().remaining > 0)
			{
				// SCALE ALPHA CHANNEL 
				// if e has 100 total / 25 remaining, alpha set to (float) remaining / total * 255 
				
				e -> get<CLifespan>().remaining--;
			}
			
			if (e -> get<CLifespan>().remaining <= 0)
			{
				e -> destroy();
			}
		}
	}
}

void Game::sEnemySpawner()
{
	if (m_currentFrame >= m_lastEnemySpawnTime + m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	// TODO: implement proper collisions between entities using collision radius, not shape radius
	
	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			spawnSmallEnemies(e);
		}
		
		for (auto e : m_entities.getEntities("small_enemy"))
		{
			
		}
	}
}

void Game::sMovement()
{	
	// allow for multiple things to affect player movement, not just user input
	// would allow for easily adding wind to player (if windy == true) {inputVelocity += 5} etc 
	Vec2f inputVelocity(0, 0);
	
	if (player() -> get<CInput>().up == true)
	{
		inputVelocity.y -= 5;
	}
	
	if (player() -> get<CInput>().down == true)
	{
		inputVelocity.y += 5;
	}
	
	if (player() -> get<CInput>().left == true)
	{
		inputVelocity.x -= 5;
	}
	
	if (player() -> get<CInput>().right == true)
	{
		inputVelocity.x += 5;
	}
	
	player() -> get<CTransform>().velocity = inputVelocity;
	
	for (auto e : m_entities.getEntities())
	{
		e -> get<CTransform>().pos += e -> get<CTransform>().velocity;
	}
}

void Game::sRender()
{
	if (!m_window.isOpen())
	{
		return;
	}
	
	m_window.clear();
	
	for (auto e : m_entities.getEntities())
	{
		e -> get<CShape>().circle.setPosition(e -> get<CTransform>().pos);
		e -> get<CTransform>().angle += 1.0f;
		e -> get<CShape>().circle.setRotation(sf::degrees(e -> get<CTransform>().angle));
	
		m_window.draw(e -> get<CShape>().circle);
	}
	
	m_window.display();
}

void Game::sUserInput()
{
	while (auto event = m_window.pollEvent())
	{
		if (event -> is<sf::Event::Closed>())
		{
			std::exit(0);
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::W)
			{
				player() -> get<CInput>().up = true;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::S)
			{
				player() -> get<CInput>().down = true;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::A)
			{
				player() -> get<CInput>().left = true;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::D)
			{
				player() -> get<CInput>().right = true;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::Escape)
			{
				std::exit(0);
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::P)
			{
				setPaused(m_paused);
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyReleased>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::W)
			{
				player() -> get<CInput>().up = false;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyReleased>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::S)
			{
				player() -> get<CInput>().down = false;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyReleased>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::A)
			{
				player() -> get<CInput>().left = false;
			}
		}
		
		if (const auto * keyPressed = event -> getIf<sf::Event::KeyReleased>())
		{
			if (keyPressed -> scancode == sf::Keyboard::Scancode::D)
			{
				player() -> get<CInput>().right = false;
			}
		}
		
		if (const auto * mousePressed = event -> getIf<sf::Event::MouseButtonPressed>())
		{
			Vec2f mousePos(mousePressed -> position);
			
			if (mousePressed -> button == sf::Mouse::Button::Left)
			{
				spawnBullet(player(), mousePos);
				
				std::cout << "Left mouse (" << mousePos.x << ", " << mousePos.y << ")\n";
			}
			
			if (mousePressed -> button == sf::Mouse::Button::Right)
			{
				// call spawnBullet() here
				
				std::cout << "Right mouse (" << mousePos.x << ", " << mousePos.y << ")\n";
			}
		}
	}
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
			fin >> m_bulletConfig.L;
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