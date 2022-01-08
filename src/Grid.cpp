#include "Grid.h"

Grid::Grid(const int &gWidth, const int &gHeight)
	: m_iWidth(gWidth), m_iHeight(gHeight)
{
	// Initializing the needed data
	m_bUpdating = false;
	m_cells = new Cell[gWidth * gHeight];

	// Setting the default values for all the cells
	for (int x = 0; x < m_iWidth; x++)
		for (int y = 0; y < m_iHeight; y++)
		{
			int index = y * m_iWidth + x;
			m_cells[index].alive = false;
			m_cells[index].x = x;
			m_cells[index].y = y;

			// Attaching to each cell the surrounding cells
			AttachSurrCells(x, y);
		}
}

void Grid::Update()
{
	if (m_bUpdating)
	{
		// This is where every iteration happens
		for (int x = 0; x < m_iWidth; x++)
			for (int y = 0; y < m_iHeight; y++)
			{
				UpdateCellState(x, y);
			}
		// Killing what should be killed
		for (auto c : m_killingVec)
			c->alive = false;
		// Reviving what should be revived
		for (auto c : m_revivingVec)
		{
			c->alive = true;
		}

		m_killingVec.clear();
		m_revivingVec.clear();
	}
}

void Grid::Stop()
{
	m_bUpdating = false;
}

void Grid::Start()
{
	m_bUpdating = true;
}

bool Grid::IsUpdating()
{
	return m_bUpdating;
}

Cell *Grid::GetCell(const int &index)
{
	if (index > -1 && index < m_iHeight * m_iWidth)
		return &m_cells[index];
	return nullptr;
}

Cell *Grid::GetCell(const int &x, const int &y)
{
	if (x >= 0 && x <= m_iWidth - 1 && y >= 0 && y <= m_iHeight - 1)
		return &m_cells[y * m_iWidth + x];
	return nullptr;
}

Grid::~Grid()
{
	delete[] m_cells;
}
void Grid::AttachSurrCells(const int &x, const int &y)
{
	int index = y * m_iWidth + x;
	// Attach each cells' arounds
	m_cells[index].aroundCells[Positions::Up] = GetCell(x, y - 1);
	m_cells[index].aroundCells[Positions::Down] = GetCell(x, y + 1);
	m_cells[index].aroundCells[Positions::Right] = GetCell(x + 1, y);
	m_cells[index].aroundCells[Positions::Left] = GetCell(x - 1, y);

	// Diagonal
	m_cells[index].aroundCells[Positions::UpRight] = GetCell(x + 1, y - 1);
	m_cells[index].aroundCells[Positions::UpLeft] = GetCell(x - 1, y - 1);
	m_cells[index].aroundCells[Positions::DownRight] = GetCell(x + 1, y + 1);
	m_cells[index].aroundCells[Positions::DownLeft] = GetCell(x - 1, y + 1);
}

void Grid::UpdateCellState(const int &x, const int &y)
{
	Cell *currentCell = GetCell(x, y);
	int aliveSurrCells = CountAlive(x, y);

	if (currentCell->alive)
	{
		if (aliveSurrCells < 2)
		{
			m_killingVec.push_back(currentCell);
		}
		// we can take this away. it is alive anyway
		//else if (aliveSurrCells == 2 || aliveSurrCells == 3)
		//{
		//	m_revivingVec.push_back(currentCell);
		//}
		else if (aliveSurrCells > 3)
		{
			m_killingVec.push_back(currentCell);
		}
	}
	else
	{
		if (aliveSurrCells == 3)
		{
			m_revivingVec.push_back(currentCell);
		}
	}
}

int Grid::CountAlive(const int &x, const int &y)
{
	int counter = 0;
	Cell *c = GetCell(x, y);

	for (int i = 0; i < 8; i++)
	{
		Cell *surr = c->aroundCells[i];
		if (surr != nullptr)
		{
			if (surr->alive)
				counter++;
		}
	}

	return counter;
}