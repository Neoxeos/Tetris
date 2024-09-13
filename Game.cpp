#define _USE_MATH_DEFINES

#include "Game.h"

#include <cmath>
#include <iostream>
#include <fstream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	/* initialize random seed: */
	srand(time(NULL));

	// read file
	std::ifstream fin(path);

	// reading for window 
	std::string name;
	int windowHeight;
	int windowWidth;
	int frameSpeed;
	int fullscreen;
	fin >> name >> windowHeight >> windowWidth >> frameSpeed >> fullscreen;

	// reading for font
	std::string fontFilePath;
	int fontSize;
	int fontRedColor;
	int fontGreenColor;
	int fontBlueColor;
	fin >> name >> fontFilePath >> fontSize >> fontRedColor >> fontGreenColor >> fontBlueColor;

	// reading for player
	fin >> name >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;

	// reading for enemies
	fin >> name >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SP;
	// set up spwn time for enemies
	m_enemySpawnInterval = m_enemyConfig.SP;

	// reading for bullet 
	fin >> name >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
	
	// set up window parameters
	if (fullscreen)
	{
		m_window.create(sf::VideoMode(windowHeight, windowWidth), "Assigment 2", sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(windowHeight, windowWidth), "Assigment 2");
	}
	m_window.setFramerateLimit(frameSpeed);

	// set up font 
	if (!m_font.loadFromFile(fontFilePath))
	{
	    std::cerr << "Could not load font!\n";
	    exit(-1);
	}
	m_text.setFont(m_font);
	m_text.setCharacterSize(fontSize);
	m_text.setFillColor(sf::Color(fontRedColor, fontGreenColor, fontRedColor));
	m_text.setString(std::string("Score: ") + std::to_string(m_score));
	m_text.setPosition(0, 0);

	ImGui::SFML::Init(m_window);

	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{
		// update entity manager
		m_entities.update();

		// required update call to  imgui
		ImGui::SFML::Update(m_window, m_deltaClock.restart());

		if (m_paused)
		{
			sUserInput();
			sGUI();
			sRender();
		}
		else
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
			sUserInput();
			sGUI();
			sRender();
			// increase current frame
			m_currentFrame++;
		}
	}
}

void Game::setPaused(bool paused)
{
	m_paused = true;
}

/* This functions spawns in teh player in the middle of the screen */
void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spawns at middle window with velocity specified by config and angle 0
	// get window size to spawn at middle
	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);

	// the entity shape will have parameters accorind to config scpecs
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	entity->cLifespan = std::make_shared<CLifespan>(0);

	// add an input component to the player so that we can use inputs, only for player
	entity->cInput = std::make_shared<CInput>();

	// since we want this entity to be our player, set our game's player variable to be this entity
	// this goes slightly against the entitymanager paradigm, but we use the player so much it's worht it
	m_player = entity;
}

/*
* This functions correctly spawns enemy inside game boundary 
*/
void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");
	float angle = 0.0f;

	// set up position for enemy
	float ex = m_enemyConfig.SR + (rand() / (float(RAND_MAX) / (m_window.getSize().x - 2*m_enemyConfig.SR))); // random number between radius and maxwindow_size-radius
	float ey = m_enemyConfig.SR + (rand() / (float(RAND_MAX) / (m_window.getSize().y - 2*m_enemyConfig.SR)));

	// set up velocity
	float vx = m_enemyConfig.SMIN + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(m_enemyConfig.SMAX - m_enemyConfig.SMIN)));
	float vy = m_enemyConfig.SMIN + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(m_enemyConfig.SMAX - m_enemyConfig.SMIN)));
	if (rand() % 2 == 0) // 50% chance to be negative 
	{
		vx = -vx;
		vy = -vy;
	}

	// set vertices
	int enemyVertices = m_enemyConfig.VMIN + (rand() / (RAND_MAX / (m_enemyConfig.VMAX - m_enemyConfig.VMIN)));
	
	// set color
	int enemyRedColor = 1 + rand() % 255;
	int enemyGreenColor = 1 + rand() % 255;
	int enemyBlueColor = 1 + rand() % 255;

	// construct enemy
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(vx, vy), angle);
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, enemyVertices, sf::Color(enemyRedColor, enemyGreenColor, enemyBlueColor), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	entity->cLifespan = std::make_shared<CLifespan>(0);

	// set score component
	entity->cScore = std::make_shared<CScore> (enemyVertices * 100);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

/* This function spawns in small enemies after we have destroyed the main one*/
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// TODO spawn small enemies at the location of the input enemy e

	// whene we create the smaller enemy we have to read the values of the orignal enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - samll enemies are worth double points of the original enemy
	for (int i = 0; i < e->cShape->circle.getPointCount(); i++)
	{
		auto enemy = m_entities.addEntity("small enemy");

		// get angle
		float angle = M_PI * (((360 * i) / e->cShape->circle.getPointCount()) + e->cTransform->angle) / 180;

		// get position
		float ex = e->cTransform->pos.x + (e->cShape->circle.getRadius() * cosf(angle));
		float ey = e->cTransform->pos.y + (e->cShape->circle.getRadius() * sinf(angle));

		// get velocity
		float speed = sqrtf(e->cTransform->velocity.x * e->cTransform->velocity.x + e->cTransform->velocity.y * e->cTransform->velocity.y);
		float vx = speed * cosf(angle);
		float vy = speed * sinf(angle);

		enemy->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(vx, vy), 0.0f);
		enemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius() / 2, e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor() , e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		enemy->cCollision = std::make_shared<CCollision>(e->cShape->circle.getRadius() / 2);
		enemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		enemy->cScore = std::make_shared<CScore>(e->cShape->circle.getPointCount() * 200);
	}
	


}

/* spawn bullet from player character to where we are clicking */
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bullet = m_entities.addEntity("bullet");

	Vec2 D = entity->cTransform->pos - target;
	Vec2 DNormal = D.normalize();
	float angle = atan2f(D.y, D.x);

	bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(DNormal.x * m_bulletConfig.S, DNormal.y * m_bulletConfig.S), angle);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V , sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cLifespan = std::make_shared <CLifespan>(m_bulletConfig.L);
}

/* spawns in special weapon */
void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO implement
}

void Game::sMovement()
{
	// movement for enemies
	for (auto& e : m_entities.getEntities())
	{
		if (e != m_player)
		{
			e->cTransform->pos.x += e->cTransform->velocity.x;
			e->cTransform->pos.y += e->cTransform->velocity.y;
		}
	}

	// movement for player
	if (m_player->cInput->up)
	{
		m_player->cTransform->pos.y -= m_player->cTransform->velocity.y;
	}
	if (m_player->cInput->left)
	{
		m_player->cTransform->pos.x -= m_player->cTransform->velocity.x;
	}
	if (m_player->cInput->down)
	{
		m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
	}
	if (m_player->cInput->right)
	{
		m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	}

	// if we want continuous movement
	//m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	//m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

/* handles the lifspan of all object with lifespan such that total lifespan > 0 */
void Game::sLifespan()
{
	for (auto& entity : m_entities.getEntities())
	{
		if (entity->cLifespan->total != 0) // has lifespan
		{
			entity->cLifespan->remaining -= 1;
			if (entity->cLifespan->remaining == 0) //dead
			{
				entity->destroy();
			}
			else // still alive
			{
				// scale alpha channel
				entity->cShape->circle.setFillColor(sf::Color(entity->cShape->circle.getFillColor().r, entity->cShape->circle.getFillColor().g, entity->cShape->circle.getFillColor().b, 255 * (float(entity->cLifespan->remaining) / float(entity->cLifespan->total))));
			}
		}
	}
}

/* Detects and handles collisions between all entities*/
void Game::sCollision()
{
	for (auto& bullet : m_entities.getEntities("bullet"))
	{
		for (auto& enemy : m_entities.getEntities("enemy"))
		{
			if (bullet->cTransform->pos.dist(enemy->cTransform->pos) < (bullet->cCollision->radius + enemy->cCollision->radius))
			{
				m_score += enemy->cScore->score;
				spawnSmallEnemies(enemy);
				enemy->destroy();
				bullet->destroy();
			}
		}
		for (auto& sEnemy : m_entities.getEntities("small enemy"))
		{
			if (bullet->cTransform->pos.dist(sEnemy->cTransform->pos) < (bullet->cCollision->radius + sEnemy->cCollision->radius))
			{
				m_score += sEnemy->cScore->score;
				sEnemy->destroy();
				bullet->destroy();
			}
		}
	}

	// Collisions betweeen walls for enemies only
	for (auto& enemy : m_entities.getEntities("enemy"))
	{
		if (enemy->cTransform->pos.x <= enemy->cCollision->radius)
		{
			enemy->cTransform->velocity.x = -enemy->cTransform->velocity.x;
		}
		if (enemy->cTransform->pos.y <= enemy->cCollision->radius)
		{
			enemy->cTransform->velocity.y = -enemy->cTransform->velocity.y;
		}
		if (enemy->cTransform->pos.x + enemy->cCollision->radius >= m_window.getSize().x)
		{
			enemy->cTransform->velocity.x = -enemy->cTransform->velocity.x;
		}
		if (enemy->cTransform->pos.y + enemy->cCollision->radius >= m_window.getSize().y)
		{
			enemy->cTransform->velocity.y = -enemy->cTransform->velocity.y;
		}
	}

	// Collisions between walls for palyer character 
	if (m_player->cTransform->pos.x <= m_player->cCollision->radius)
	{
		m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	}
	if (m_player->cTransform->pos.y <= m_player->cCollision->radius)
	{
		m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
	}
	if (m_player->cTransform->pos.x >= (m_window.getSize().x - m_player->cCollision->radius))
	{
		m_player->cTransform->pos.x -= m_player->cTransform->velocity.x;
	}
	if (m_player->cTransform->pos.y >= (m_window.getSize().y - m_player->cCollision->radius))
	{
		m_player->cTransform->pos.y -= m_player->cTransform->velocity.y;
	}
	
	// Collision between player and enemies
	for (auto& enemy : m_entities.getEntities("enemy"))
	{
		if (enemy->cTransform->pos.dist(m_player->cTransform->pos) < (m_player->cCollision->radius + enemy->cCollision->radius))
		{
			enemy->destroy();
			m_player->cTransform->pos.x = m_window.getSize().x / 2.0f;
			m_player->cTransform->pos.y = m_window.getSize().y / 2.0f;
		}
	}
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemySpawnInterval)
	{
		spawnEnemy();
	}
}

void Game::sGUI()
{
	ImGui::Begin("Geometry Wars");
	ImGui::Text("Change Settings Here");
	ImGui::End();
}

void Game::sRender()
{
	m_window.clear();

	for (auto& entity : m_entities.getEntities())
	{
		// set position of the shape based on the entity's transform->pos
		entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);

		// set the rotation of the shape based on the eneity's transform->angle
		entity->cTransform->angle += 1.0f;
		entity->cShape->circle.setRotation(entity->cTransform->angle);

		//draw the entity's sf::CirlceShape
		m_window.draw(entity->cShape->circle);
	}
	// update score
	m_text.setString(std::string("Score: ") + std::to_string(m_score));
	m_window.draw(m_text);

	// draw the ui last
	ImGui::SFML::Render(m_window);

	m_window.display();
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// pass the event to imgui to be parsed
		ImGui::SFML::ProcessEvent(m_window, event);

		// this event triggers whene the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// Pressing 'P' pauses the game
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				m_paused = !m_paused;
			}
		}

		// Pressing 'ESC' closes the game
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_running = false;
				ImGui::SFML::Shutdown();
			}
		}

		if (!m_paused)
		{
			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					//std::cout << "W is pressed\n";
					m_player->cInput->up = true;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is released
			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					//std::cout << "W is released\n";
					m_player->cInput->up = false;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::S:
					//std::cout << "S is pressed\n";
					m_player->cInput->down = true;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is released
			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::S:
					//std::cout << "S is released\n";
					m_player->cInput->down = false;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::A:
					//std::cout << "A is pressed\n";
					m_player->cInput->left = true;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is released
			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::A:
					//std::cout << "A is released\n";
					m_player->cInput->left = false;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::D:
					//std::cout << "D is pressed\n";
					m_player->cInput->right = true;
					break;
				default: break;
				}
			}

			// this event is triggered when a key is released
			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::D:
					//std::cout << "D is released\n";
					m_player->cInput->right = false;
					break;
				default: break;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				// this line ignores mouse events if ImGui is the thing beign clicked 
				if (ImGui::GetIO().WantCaptureMouse) { continue; }

				if (event.mouseButton.button == sf::Mouse::Left)
				{
					/*std::cout << "Left Mouse is Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << "\n";*/
					// call spawn bullet here
					Vec2 target = Vec2(event.mouseButton.x, event.mouseButton.y);
					spawnBullet(m_player, target);
				}

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "Right Mouse is Clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << "\n";
					// call special weapon here
				}
			}
		}
	}
}