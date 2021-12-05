#include "Map.hpp"
#include <stdexcept>
#include <sstream>

Map::Map(std::size_t size)
	: _size(size + 1)
{
	if (_size == 0)
	{
		throw std::runtime_error("Invalid map size");
	}
	_map.resize(_size * _size, 0);
}

void Map::DrawHorizontalLine(const Line &line)
{
	const auto linePoints = line.GetPoints();
	const auto begin = std::min(linePoints[0].first, linePoints[1].first);
	const auto end = std::max(linePoints[0].first, linePoints[1].first);
	const auto y = linePoints[0].second;

	for (auto pos = begin; pos <= end; ++pos)
	{
		_map[GetPointIndex(pos, y)] += 1;
	}
}

void Map::DrawVerticalLine(const Line &line)
{
	const auto linePoints = line.GetPoints();
	const auto begin = std::min(linePoints[0].second, linePoints[1].second);
	const auto end = std::max(linePoints[0].second, linePoints[1].second);
	const auto x = linePoints[0].first;

	for (auto pos = begin; pos <= end; ++pos)
	{
		_map[GetPointIndex(x, pos)] += 1;
	}
}

std::size_t Map::GetPointIndex(std::size_t x, std::size_t y) const
{
	return y * _size + x;
}

std::size_t Map::CountIntersections() const
{
	std::size_t result {};
	for (const auto &point: _map)
	{
		if (point >= 2)
		{
			result += 1;
		}
	}
	return result;
}

std::string Map::DrawMap() const
{
	std::ostringstream stream;
	for (auto y = 0U; y < _size; ++y)
	{
		for (auto x = 0U; x < _size; ++x)
		{
			const auto val = _map[GetPointIndex(x, y)];
			if (val == 0)
			{
				stream << ".";
			}
			else
			{
				stream << static_cast<int>(val);
			}
		}
		stream << "\r\n";
	}
	return stream.str();
}

void Map::DrawLine(const Line &line)
{
	if (line.IsHorizontal())
	{
		DrawHorizontalLine(line);
	}
	else if (line.IsVertical())
	{
		DrawVerticalLine(line);
	}
	else
	{
		DrawDiagonalLine(line);
	}
}

void Map::DrawDiagonalLine(const Line &line)
{
	const auto &[x1, y1] = line.GetPoints()[0];
	const auto &[x2, y2] = line.GetPoints()[1];

	const auto xDiff = std::abs(x1 - x2);
	const auto yDiff = std::abs(y1 - y2);

	if (xDiff != yDiff)
	{
		throw std::runtime_error("Line is not angled at 45 degrees");
	}

	const auto xStep = (x1 > x2) ? -1 : 1;
	const auto yStep = (y1 > y2) ? -1 : 1;

	auto x = x1;
	auto y = y1;

	for (auto i = 0; i <= yDiff; ++i)
	{
		_map[GetPointIndex(x, y)] += 1;
		x += xStep;
		y += yStep;
	}
}


