#include "Line.hpp"

Line::Line(const Point &p1, const Point &p2)
	: _points({p1, p2})
{
}

bool Line::IsHorizontal() const
{
	return _points[0].second == _points[1].second;
}

bool Line::IsVertical() const
{
	return _points[0].first == _points[1].first;
}

const std::array<std::pair<std::uint16_t, std::uint16_t>, 2> &Line::GetPoints() const
{
	return _points;
}
