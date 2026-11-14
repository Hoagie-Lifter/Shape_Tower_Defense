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
	
	m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2",  sf::State::Fullscreen);
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);
	
	spawnWalls();
}

std::shared_ptr<Entity> Game::tower()
{
	return m_entities.getEntities("tower").back();
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
			shootBullet();
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

void Game::spawnWalls()
{
	auto e1 = m_entities.addEntity("wall");
	
	e1 -> add<CRectangle>(
	sf::Color(m_wallConfig.FR, m_wallConfig.FG, m_wallConfig.FB),
	sf::Color(m_wallConfig.OR, m_wallConfig.OG, m_wallConfig.OB),
	m_wallConfig.W,
	m_wallConfig.H,
	m_wallConfig.OT);
	
	e1 -> add<CTransform>(Vec2f{m_windowConfig.W / 2, -50});
	
	auto e2 = m_entities.addEntity("wall");
	
	e2 -> add<CRectangle>(
	sf::Color(m_wallConfig.FR, m_wallConfig.FG, m_wallConfig.FB),
	sf::Color(m_wallConfig.OR, m_wallConfig.OG, m_wallConfig.OB),
	m_wallConfig.W,
	m_wallConfig.H,
	m_wallConfig.OT);
	
	e2 -> add<CTransform>(Vec2f{m_windowConfig.W / 2, m_windowConfig.H + 50});
}

void Game::spawnTower(const Vec2f & mousePos)
{
	auto e = m_entities.addEntity("tower");
	
	e -> add<CInput>();
	e -> add<CCircle>(
	m_towerConfig.SR,
	m_towerConfig.V,
	sf::Color(m_towerConfig.FR, m_towerConfig.FG, m_towerConfig.FB),
	sf::Color(m_towerConfig.OR, m_towerConfig.OG, m_towerConfig.OB),
	m_towerConfig.OT);
	
	e -> add<CTransform>(Vec2f(mousePos.x, mousePos.y));
	
	e -> add<CTargeting>(200, 1000, sf::Color(100, 100, 100, 50));
}

void Game::spawnEnemy()
{
	auto e = m_entities.addEntity("enemy");
	
	e -> add<CCircle>(
	m_enemyConfig.SR,
	m_enemyConfig.VMAX,
	sf::Color(0, 0, 255),
	sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
	m_enemyConfig.OT);
	
	e -> add<CTransform>(Vec2f(-100, m_windowConfig.H / 2), Vec2f(5, 0), 0);
	e -> add<CLifespan>(1500);
	
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	//TODO
}

void Game::shootBullet()
{
	// TODO: check if (e -> has<CTargeting>())
	
	for (auto e : m_entities.getEntities("tower"))
	{
		if (m_currentFrame % 50 == 0)
		{
			spawnBullet(e, Vec2f{100, 100});
		}
	}
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
	
	e -> add<CCircle>(
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
		Vec2f bulletPos = b ->get<CTransform>().pos;
		
		for (auto e : m_entities.getEntities("enemy"))
		{
			Vec2f entityPos = e ->get<CTransform>().pos;
			
			if (bulletPos.distance(entityPos) <= m_bulletConfig.SR + m_enemyConfig.SR)
			{
				b -> destroy();
				e -> destroy();
			}
		}
		
		for (auto e : m_entities.getEntities("small_enemy"))
		{
			
		}
	}
	
	//for (auto e : m_entities.getEntities("enemy"))
	//{
	//	Vec2f entityPos = e ->get<CTransform>().pos;
	//		
	//	if (entityPos.x <= m_windowConfig.W)
	//	{
	//		e -> destroy();
	//		std::cout << "Enemy made it to the end!\n";
	//		//m_remainingHealth--;
	//	}
	//}
	
	for (auto e : m_entities.getEntities("tower"))
	{
		Vec2f mousePos = sf::Mouse::getPosition(m_window);
		Vec2f towerPos = e -> get<CTransform>().pos;

		e -> get<CTargeting>().displayed = (mousePos.distance(towerPos) <= m_towerConfig.SR);
	}
}

void Game::sMovement()
{	
	// allow for multiple things to affect tower movement, not just user input
	// would allow for easily adding wind to tower (if windy == true) {inputVelocity += 5} etc 
	
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
	
	for (auto e : m_entities.getEntities("wall"))
	{
		e -> get<CRectangle>().rectangle.setPosition(e -> get<CTransform>().pos);
		m_window.draw(e -> get<CRectangle>().rectangle);
	}
	
	for (auto e : m_entities.getEntities("tower"))
	{
		e -> get<CCircle>().circle.setPosition(e -> get<CTransform>().pos);
		m_window.draw(e -> get<CCircle>().circle);

		if (e -> get<CTargeting>().displayed == true)
		{
			e -> get<CTargeting>().range.setPosition(e -> get<CTransform>().pos);
			m_window.draw(e -> get<CTargeting>().range);				
		}
	}
	
	for (auto e : m_entities.getEntities("enemy"))
	{
		e -> get<CCircle>().circle.setPosition(e -> get<CTransform>().pos);
		e -> get<CTransform>().angle += 1.0f;
		e -> get<CCircle>().circle.setRotation(sf::degrees(e -> get<CTransform>().angle));
		m_window.draw(e -> get<CCircle>().circle);
	}
	
	for (auto e : m_entities.getEntities("bullet"))
	{
		e -> get<CCircle>().circle.setPosition(e -> get<CTransform>().pos);
		m_window.draw(e -> get<CCircle>().circle);
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
		
		if (const auto * mousePressed = event -> getIf<sf::Event::MouseButtonPressed>())
		{
			Vec2f mousePos(mousePressed -> position);
			
			if (mousePressed -> button == sf::Mouse::Button::Left)
			{
				spawnTower(mousePos);
			}
			
			if (mousePressed -> button == sf::Mouse::Button::Right)
			{
				
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
		
		if (token == "Bullet:")
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
		
		if (token == "Enemy:")
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
		
		if (token == "Tower:")
		{
			// TowerConfig {int SR, CR, OT, V, FR, FG, FB, OR, OG, OB;};
			
			fin >> m_towerConfig.SR;
			fin >> m_towerConfig.CR;
			fin >> m_towerConfig.OT;
			fin >> m_towerConfig.V;
			fin >> m_towerConfig.FR;
			fin >> m_towerConfig.FG;
			fin >> m_towerConfig.FB;
			fin >> m_towerConfig.OR;
			fin >> m_towerConfig.OG;
			fin >> m_towerConfig.OB;
		}
		
		if (token == "Wall:")
		{
			// WallConfig {int SR, CR, OT, V, FR, FG, FB, OR, OG, OB;};
			
			fin >> m_wallConfig.W;
			fin >> m_wallConfig.H;
			fin >> m_wallConfig.OT;
			fin >> m_wallConfig.FR;
			fin >> m_wallConfig.FG;
			fin >> m_wallConfig.FB;
			fin >> m_wallConfig.OR;
			fin >> m_wallConfig.OG;
			fin >> m_wallConfig.OB;
		}
	}
}