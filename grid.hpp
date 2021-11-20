#pragma once

#include <ostream>
#include <cstdint>

class GridCell {
	uint8_t m_value;

public:
	static constexpr int BOMB = 9;
	
	GridCell();
	
	void SetFlag(bool v);
	bool HasFlag() const;
	void SetVisible(bool v);
	bool IsVisible() const;
	
	void SetValue(uint8_t value);
	uint8_t GetValue() const;
};

class Grid {
	friend std::ostream &operator<<(std::ostream &, const Grid &);
public:
	Grid(uint8_t w, uint8_t h);
	~Grid();
	
	void FillRandom(float bombsPercent);
	void UncoverAll();
	bool IsAllSafe();
	void FloodFillUncover(int x, int y);
	
	GridCell &At(uint8_t x, uint8_t y);
	const GridCell &At(uint8_t x, uint8_t y) const;
private:
	GridCell *m_grid;
	uint8_t m_width, m_height;
};

std::ostream &operator<<(std::ostream &stream, const Grid &grid);