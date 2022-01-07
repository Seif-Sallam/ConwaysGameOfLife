#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

enum Positions {
	Up = 0,
	Down,
	Right,
	Left,
	UpRight,
	UpLeft,
	DownRight,
	DownLeft
};


struct Cell {
	bool alive = false;					// The cell is either Alive or Dead
	Cell* aroundCells[8]{ nullptr };    // The cells around the current cell
	int x, y;							// The position in the 2D Space
};

class Grid
{
public:
	Grid(const int& gWidth, const int& gHeight);
	void Update();

	void Stop();			// Stop simulation
	void Start();			// Start Simulation
	bool IsUpdating();		// Check if the simulation is working
	
	Cell* GetCell(const int& index);
	Cell* GetCell(const int& x, const int& y);
	~Grid();
private:
	void AttachSurrCells(const int& x, const int& y);	// Updating the surrounding Cells
	void UpdateCellState(const int& x, const int& y);	// Updating whether the cell is alive or dead
	int CountAlive(const int& x, const int& y);			// Counting the alive cells around the one cell

private:

	bool m_bUpdating;
	const int m_iHeight;
	const int m_iWidth;
	Cell* m_cells;
	std::vector<Cell*> m_killingVec;		// vector holding the cells that will die next generation
	std::vector<Cell*> m_revivingVec;		// vector holding the cells that will stay alive and will live next generation
};
