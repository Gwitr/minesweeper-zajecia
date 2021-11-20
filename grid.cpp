#include <stdexcept>
#include <vector>
#include <utility>
#include <iostream>
#include "grid.hpp"

void GridCell::SetFlag(bool v) {
	if (v) {
		m_value |= 0b10000000;
	} else {
		m_value &= 0b01111111;
	}
}
bool GridCell::HasFlag() const {
	return m_value & 0b10000000;
}
void GridCell::SetVisible(bool v) {
	if (v) {
		m_value |= 0b01000000;
	} else {
		m_value &= 0b10111111;
	}
}
bool GridCell::IsVisible() const {
	return m_value & 0b01000000;
}
void GridCell::SetValue(uint8_t value) {
	m_value = value & 0b00111111;
}
uint8_t GridCell::GetValue() const {
	return m_value & 0b00111111;
}
GridCell::GridCell() {
	m_value = 0;
}

Grid::Grid(uint8_t w, uint8_t h) {
	m_width = w;
	m_height = h;
	m_grid = new GridCell[w * h];
}

void Grid::FillRandom(float bombsPercent) {
	for (int i = 0; i < m_width * m_height; i++) m_grid[i] = GridCell();
	
	std::vector<std::pair<int, int>> bombCoordinates;
	
	// Dodaj bomby
	int nbombs = 0;
	while (nbombs < m_width * m_height * bombsPercent / 100) {
		int x = (float)rand() / RAND_MAX * (m_width + 0.5f);
		int y = (float)rand() / RAND_MAX * (m_height + 0.5f);
		
		if (x >= m_width) x = m_width - 1;
		if (y >= m_height) y = m_height - 1;
		
		auto &cell = At(x, y);
		if (cell.GetValue() != GridCell::BOMB) {
			bombCoordinates.push_back({ x, y });
			cell.SetValue(GridCell::BOMB);
			nbombs++;
		}
	};
	
	// Popraw liczby
	for (const auto &pair : bombCoordinates) {
		for (int ox = -1; ox < 2; ox++) {
			if (ox == -1 && pair.first == 0) continue;
			if (ox == 1 && pair.first == m_width - 1) continue;
			for (int oy = -1; oy < 2; oy++) {
				if (oy == -1 && pair.second == 0) continue;
				if (oy == 1 && pair.second == m_height - 1) continue;
				if (ox == 0 && oy == 0) continue;
				
				auto &cell = At(pair.first + ox, pair.second + oy);
				if (cell.GetValue() != GridCell::BOMB) {
					cell.SetValue(cell.GetValue() + 1);
				}
			}
		}
	}
}

void Grid::UncoverAll() {
	for (int i = 0; i < m_width * m_height; i++)
		m_grid[i].SetVisible(true);
}

void Grid::FloodFillUncover(int x, int y) {
	At(x, y).SetVisible(true);
	if (At(x, y).GetValue() == 0) {
		if (x != 0 && !At(x-1, y).IsVisible())
			FloodFillUncover(x-1, y);
		if (y != 0 && !At(x, y-1).IsVisible())
			FloodFillUncover(x, y-1);
		if (y < m_height-1 && !At(x, y+1).IsVisible())
			FloodFillUncover(x, y+1);
		if (x < m_width-1 && !At(x+1, y).IsVisible())
			FloodFillUncover(x+1, y);
	}
}

bool Grid::IsAllSafe() {
	for (int i = 0; i < m_width * m_height; i++) {
		if (!(m_grid[i].IsVisible() || (m_grid[i].HasFlag() && m_grid[i].GetValue() == GridCell::BOMB))) {
			return false;
		}
	}
	return true;
}

Grid::~Grid() {
	delete m_grid;
}

GridCell &Grid::At(uint8_t x, uint8_t y) {
	if (x >= m_width || y >= m_height) throw std::out_of_range("Grid index out of range");
	return m_grid[y * m_width + x];
}
const GridCell &Grid::At(uint8_t x, uint8_t y) const {
	if (x >= m_width || y >= m_height) throw std::out_of_range("Grid index out of range");
	return m_grid[y * m_width + x];
}

std::ostream &operator<<(std::ostream &stream, const Grid &grid) {
	stream << "  ";
	for (int column = 1; column <= grid.m_width; column++) {
		stream << column << " ";
	}
	
	stream << "\n1 ";
	for (int i = 0; i < grid.m_width * grid.m_height; i++) {
		if (i % grid.m_width == 0 && i != 0)
			stream << "\n" << i / grid.m_width + 1 << " ";
		auto &cell = grid.m_grid[i];
		if (cell.HasFlag()) {
			stream << "P";
		} else if (cell.IsVisible()) {
			if (cell.GetValue() == GridCell::BOMB) {
				stream << "#";
			} else if (cell.GetValue() == 0) {
				stream << "_";
			} else {
				stream << (int)cell.GetValue();
			}
		} else {
			stream << ".";
		}
		stream << " ";
	}
	return stream;
}