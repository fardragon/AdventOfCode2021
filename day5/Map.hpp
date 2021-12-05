#ifndef ADVENTOFCODE2021_MAP_HPP
#define ADVENTOFCODE2021_MAP_HPP

#include <cstdint>
#include <vector>
#include <string>
#include "Line.hpp"

class Map
{
public:
	explicit Map(std::size_t size);
	void DrawLine(const Line &line);
	[[nodiscard]] std::size_t CountIntersections() const;
	[[nodiscard]] std::string DrawMap() const;

private:
	[[nodiscard]] std::size_t GetPointIndex(std::size_t x, std::size_t y) const;

	void DrawVerticalLine(const Line &line);
	void DrawHorizontalLine(const Line &line);
	void DrawDiagonalLine(const Line &line);
private:
	const std::size_t _size;
	std::vector<std::uint8_t> _map;
};
#endif //ADVENTOFCODE2021_MAP_HPP
