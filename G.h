#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Shape.h"

class G
{
	public: 
		sf::RenderWindow m_window; // the window we will draw to
		int m_windowHeight;
		int m_windowWidth;
		int m_Cols;
		int m_Rows;
		bool m_running = false;
		bool m_ready = true;
		bool m_GameOver = false;
		bool m_destroyToDraw = false;
		std::vector<Shape> m_Rep;
		std::vector<Shape> m_toDraw;
		std::map<int, int> m_listOfy;
		sf::Clock deltaClock;
		int currentFrame;
		int spawnFrame;
		int gametime;

		void init();
		void run();
		void display();
		void userInput();
		void movement();
		void collision();
		void logic();
		void destroy(std::vector<Shape>& target);
		void add(int dx, int dy);
		void collision(int dx, int dy);
		void update(int dx, int dy);
		void rotate(std::vector<Shape>& shapes);
};