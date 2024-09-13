#include "Grid.h"
#include "G.h"

	void Grid::init()
	{
		for (int i = 0; i < m_Rows; i++)
		{
			for (int j = 0; j < m_Cols; j++)
			{
				m_Rep[i][j] = 0;
			}
		}
	}

	void Grid::display()
	{
		for (int i = 0; i < m_Rows; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0, i * 30)),
				sf::Vertex(sf::Vector2f(600, i * 30))
			};
			G::m_window.draw(line, 2, sf::Lines);
		}

		for (int j = 0; j < m_Cols; j++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(j * 30, 0)),
				sf::Vertex(sf::Vector2f(j * 30, 300))
			};
			m_window.draw(line, 2, sf::Lines);
		}
	}