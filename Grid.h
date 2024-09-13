#pragma once
#include <vector>
#include "G.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class Grid
{
	public:
		// size of grid
		int m_Cols;
		int m_Rows;
		std::vector<std::vector<int>> m_Rep;

		void init();
		void display();
};