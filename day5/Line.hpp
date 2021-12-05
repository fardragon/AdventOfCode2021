#ifndef ADVENTOFCODE2021_LINE_HPP
#define ADVENTOFCODE2021_LINE_HPP

#include <utility>
#include <cstdint>
#include <array>

class Line
{
public:
	using Point = std::pair<std::uint16_t, std::uint16_t>;
	Line(const Point &p1, const Point &p2);

	[[nodiscard]] bool IsHorizontal() const;
	[[nodiscard]] bool IsVertical() const;

private:
	std::array<Point, 2> _points;
public:
	const std::array<std::pair<std::uint16_t, std::uint16_t>, 2> &GetPoints() const;
};

#endif //ADVENTOFCODE2021_LINE_HPP
