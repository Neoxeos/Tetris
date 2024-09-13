#include <iostream>
#include "Shape.h"
#include "G.h"


	void G::init()
	{
		srand(time(NULL));
		// init of map of y coordinates here
		for (int i = 0; i < 20; i++)
		{
			m_listOfy[51 + i * 30] = 0;
		}
		currentFrame = 0;
		spawnFrame = currentFrame;
		gametime = 60;
		m_windowHeight = 720;
		m_windowWidth = 1080;
		m_Rows = 20;
		m_Cols = 10;
		m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Tetris");
		m_window.setFramerateLimit(60);
		m_running = true;
		ImGui::SFML::Init(m_window);
		run();
	}

	void G::run()
	{
		
		while (m_running)
		{
			ImGui::SFML::Update(m_window, deltaClock.restart());
			ImGui::Begin("Geometry Wars");
			ImGui::Text("Change Settings Here");
			ImGui::End();
			currentFrame++;
			display();
			userInput();
			movement();
			if (m_GameOver)
			{
				std::cout << "Game Over";
				m_running = false;
				m_window.close();
			}
		}
	}

	void G::display()
	{
		m_window.clear(sf::Color(120, 166, 164, 100));

		// draw lines between text and grid
		sf::Vertex sep1[] =
		{
			sf::Vertex(sf::Vector2f(510, 0)),
			sf::Vertex(sf::Vector2f(510,  1080))
		};

		sf::Vertex sep2[] =
		{
			sf::Vertex(sf::Vector2f(530, 0)),
			sf::Vertex(sf::Vector2f(530,  1080))
		};
		m_window.draw(sep1, 2, sf::Lines);
		m_window.draw(sep2, 2, sf::Lines);

		// generate if we are ready to so when shape at bottom or start of game
		if (m_ready)
		{
			Shape S;
			m_toDraw = S.generate();
			m_ready = false;
		}

		// draw shape and add to m_Rep
		for (auto& shape : m_toDraw)
		{
			m_window.draw(shape);
		}

		for (auto& shape: m_Rep)
		{
			m_window.draw(shape);
		}

		// draw grid
		// grids will be 30 by 30
		// grid starts at (50,50) and ends at (350,650)
		for (int i = 0; i < m_Rows + 1; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(50, 50 + i * 30 )),
				sf::Vertex(sf::Vector2f( 350,  50 + i * 30))
			};
			m_window.draw(line, 2, sf::Lines);
		}

		for (int j = 0; j < m_Cols + 1; j++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(j * 30 + 50, 50)),
				sf::Vertex(sf::Vector2f(j * 30 + 50, 650))
			};
			m_window.draw(line, 2, sf::Lines);
		}
		ImGui::SFML::Render(m_window);
		m_window.display();
	}

	void G::collision(int dx, int dy)
	{
		// check here for collision with shapes on grid and resolve
		for (auto& shape : m_toDraw)
		{
			// calculate new position
			int newX = shape.getPosition().x + dx;
			int newY = shape.getPosition().y + dy;

			// check to see if already occupied
			for (auto& shape2 : m_Rep)
			{
				if (newX == shape2.getPosition().x && newY == shape2.getPosition().y && dy != 0) // then would occupy same spot
				{
					m_destroyToDraw = true;
					return;
				}
				if (newX == shape2.getPosition().x && newY == shape2.getPosition().y) // dy == 0
				{
					continue;
				}
			}
		}
	}


	void G::update(int dx, int dy)
	{
		// check if at end of map dx = 0 and dy = 0 represent that state
		if (dx == 0 && dy == 0)
		{
			add(0, 0);
			for (auto& shape : m_toDraw)
			{
				shape.toDestroy = true;
			}
			destroy(m_toDraw);
			m_ready = true;
			logic();
			return;
		}

		collision(dx, dy);
		if (m_destroyToDraw)
		{
			// adds to m_Rep
			add(0, 0);
			for (auto& shape : m_toDraw)
			{
				shape.toDestroy = true;
			}
			destroy(m_toDraw);
			m_destroyToDraw = false;
			m_ready = true;
			logic();
			return;
		}

		for (auto& shape : m_toDraw)
		{
			shape.setPosition(shape.getPosition().x + dx, shape.getPosition().y + dy);
		}
	}

	void G::destroy(std::vector<Shape>& target)
	{
		// takes care of iterator invalidation
		std::vector<Shape> result;
		for (auto& shape : target)
		{
			if ( !shape.toDestroy )
			{
				result.push_back(shape);
			}
		}
		target = result;
	}

	void G::add(int dx, int dy)
	{
		for (auto& shape : m_toDraw)
		{
			shape.setPosition(shape.getPosition().x + dx, shape.getPosition().y + dy);
			m_Rep.push_back(shape);
			m_listOfy[shape.getPosition().y]++;
		}
	}

	void G::rotate(std::vector<Shape>& shapes)
	{
		std::string type = shapes[0].type;
		if (type == "S")
		{
			// we do nothing
		}
		else if (type == "L" || type == "T" || type == "Line")
		{
			for (auto& shape : shapes)
			{
				if (shape.id == 11)
				{
					shape.setPosition(shape.getPosition().x + 30, shape.getPosition().y + 30);
					shape.id = 21;
				}
				else if (shape.id == 41)
				{
					shape.setPosition(shape.getPosition().x + 30, shape.getPosition().y - 30);
					shape.id = 11;
				}
				else if (shape.id == 42)
				{
					shape.setPosition(shape.getPosition().x + 60, shape.getPosition().y - 60);
					shape.id = 12;
				}
				else if (shape.id == 12)
				{
					shape.setPosition(shape.getPosition().x + 60, shape.getPosition().y + 60);
					shape.id = 22;
				}
				else if (shape.id == 21)
				{
					shape.setPosition(shape.getPosition().x - 30, shape.getPosition().y + 30);
					shape.id = 31;
				}
				else if (shape.id == 22)
				{
					shape.setPosition(shape.getPosition().x - 60, shape.getPosition().y + 60);
					shape.id = 32;
				}
				else if (shape.id == 31)
				{
					shape.setPosition(shape.getPosition().x - 30, shape.getPosition().y - 30);
					shape.id = 41;
				}
				else if (shape.id == 32)
				{
					shape.setPosition(shape.getPosition().x - 60, shape.getPosition().y - 60);
					shape.id = 42;
				}
			}
		}
	}

	void G::logic()
	{
		// always called after m_toDraw has been destroyed and m_Rep has been updated
		std::vector<int> trackY;
		for (auto& [key, vaue] : m_listOfy)
		{
			if (m_listOfy[key] == 10)
			{
				trackY.push_back(key);
				for (auto& shape : m_Rep)
				{
					if (shape.getPosition().y == key)
					{
						shape.toDestroy = true;
					}
				}
			}
		}

		destroy(m_Rep);

		if (trackY.size() > 0)
		{
			// here we drop all shapes down by one grid and also update our map rep
			for (int y : trackY)
			{
				for (auto& shape : m_Rep)
				{
					if (shape.getPosition().y < y)
					{
						shape.setPosition(shape.getPosition().x, shape.getPosition().y + 30); // drop down
					}
				}
			}

			// rebuild m_listOfY
			for (auto& [key, value] : m_listOfy)
			{
				m_listOfy[key] = 0;
			}

			for (auto& shape : m_Rep)
			{
				m_listOfy[shape.getPosition().y]++;
			}
			logic();
		}


		// check if game is over
		for (auto& shape : m_Rep)
		{
			if (shape.getPosition().y <= 81 && (shape.getPosition().x == 201 || shape.getPosition().x == 171)) // cannot spawn anymore shapes
			{
				m_GameOver = true;
				return;
			}
		}

		// now we can stop m_toDraw and spawn new shape
		m_ready = true;
	}

	void G::movement()
	{
		if (currentFrame - spawnFrame == gametime) // every second
		{
			int maxY = 50;
			for (auto& shape : m_toDraw)
			{
				if (shape.getPosition().y > maxY)
				{
					maxY = shape.getPosition().y;
				}
			}
			if (maxY < 618)
			{
				int dx = 0;
				int dy = 30;
				update(dx, dy);
			}
			else
			{
				update(0, 0);
			}
			spawnFrame = currentFrame;
		}
	}

	void G::userInput()
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

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Space:
					rotate(m_toDraw);
					break;

				default: break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				int maxX = 350;
				switch (event.key.code)
				{
					case sf::Keyboard::A:
						for (auto& shape : m_toDraw)
						{
							if (shape.getPosition().x < maxX)
							{
								maxX = shape.getPosition().x;
							}
						}

						if (maxX >= 52)
						{
							int dx= -30;
							int dy = 0;
							update(dx, dy);
							//shape.setPosition(shape.getPosition().x - 30, shape.getPosition().y);
						}
						break;

					default: break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				int maxX = 50;
				switch (event.key.code)
				{
				case sf::Keyboard::D:
					for (auto& shape : m_toDraw)
					{
						if (shape.getPosition().x > maxX)
						{
							maxX = shape.getPosition().x;
						}
					}

					if (maxX <= 320)
					{
						int dx = 30;
						int dy = 0;
						update(dx, dy);
					}
					break;

				default: break;
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				int maxY = 50;
				switch (event.key.code)
				{
				case sf::Keyboard::S:
					for (auto& shape : m_toDraw)
					{
						if (shape.getPosition().y > maxY)
						{
							maxY = shape.getPosition().y;
						}
					}
					if (maxY < 618)
					{
						int dy = 30;
						int dx = 0;
						update(dx, dy);
					}
					else
					{
						update(0, 0);
					}
					break;

				default: break;
				}
			}

		}
	}
