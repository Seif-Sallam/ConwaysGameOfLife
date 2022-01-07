#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "Grid.h"

class Demo
{
public:
	Demo(int sWidth, int sHeight);
	void Run();

private:
	void EventHandler();
	void Update();
	void Draw();

private:
	sf::RenderWindow* m_window;
	sf::Event* m_event;
	
	sf::View m_view;
	sf::Vector2f m_fOffset;
	sf::Vector2f m_fCellSize;
	sf::Vector2f m_fViewPos;

	float m_fSpeed;

	Grid* m_grid;
	int gHeight;
	int gWidth;

	sf::Clock m_clock;
	float m_fElapsedTime = 0.f;
	sf::RectangleShape* m_gridShapes;
	
	sf::RectangleShape borders[4];
};

