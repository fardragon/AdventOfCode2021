//
// Created by fardragon on 12/22/21.
//

#include "Cuboid.hpp"

#include <algorithm>

Cuboid::Cuboid(std::pair<std::int64_t, std::int64_t> x, std::pair<std::int64_t, std::int64_t> y,
               std::pair<std::int64_t, std::int64_t> z) : x(std::move(x)), y(std::move(y)), z(std::move(z))
{

}

bool Cuboid::Intersects(const Cuboid &other) const noexcept
{
	return !(
			(other.x.first > x.second)
			|| (other.x.second < x.first)
			|| (other.y.first > y.second)
			|| (other.y.second < y.first)
			|| (other.z.first > z.second)
			|| (other.z.second < z.first)
	);
}

std::optional<Cuboid> Cuboid::Intersection(const Cuboid &other) const noexcept
{
	if (this->Intersects(other))
	{
		return Cuboid
				{
						{std::max(x.first, other.x.first), std::min(x.second, other.x.second)},
						{std::max(y.first, other.y.first), std::min(y.second, other.y.second)},
						{std::max(z.first, other.z.first), std::min(z.second, other.z.second)}
				};
	}
	else
	{
		return std::nullopt;
	}
}

std::vector<Cuboid> Cuboid::RemoveIntersection(const Cuboid &other) const noexcept
{
	const auto intersection = Intersection(other);

	if (intersection.has_value())
	{
		if (intersection == *this)
		{
			return {};
		}
		else
		{
			return Remove(other);
		}
	}
	else
	{
		return {*this};
	}
}

std::vector<Cuboid> Cuboid::Remove(const Cuboid &other) const noexcept
{
	std::vector<Cuboid> result;
	result.reserve(6);

	// top part
	auto minZ = other.z.second + 1;
	if (minZ <= z.second)
	{
		result.emplace_back(Cuboid{
				{x.first, x.second},
				{y.first, y.second},
				{minZ,    z.second}}
		);
		minZ -= 1;
	}
	else
	{
		minZ = z.second;
	}

	// bottom part
	auto maxZ = other.z.first - 1;

	if (maxZ >= z.first)
	{
		result.emplace_back(Cuboid{
				{x.first, x.second},
				{y.first, y.second},
				{z.first, maxZ}}
		);
		maxZ += 1;
	}
	else
	{
		maxZ = z.first;
	}

	// right part
	auto minX = other.x.second + 1;
	if (minX <= x.second)
	{
		result.emplace_back(Cuboid{
				{minX,    x.second},
				{y.first, y.second},
				{maxZ,    minZ}}
		);
		minX -= 1;
	}
	else
	{
		minX = x.second;
	}

	// left part
	auto maxX = other.x.first - 1;
	if (maxX >= x.first)
	{
		result.emplace_back(Cuboid{
				{x.first, maxX},
				{y.first, y.second},
				{maxZ,    minZ}}
		);
		maxX += 1;
	}
	else
	{
		maxX = x.first;
	}

	// front part
	auto minY = other.y.second + 1;
	if (minY <= y.second)
	{
		result.emplace_back(Cuboid{
				{maxX, minX},
				{minY, y.second},
				{maxZ, minZ}}
		);
	}

	auto maxY = other.y.first - 1;
	if (maxY >= y.first)
	{
		result.emplace_back(Cuboid{
				{maxX,    minX},
				{y.first, maxY},
				{maxZ,    minZ}}
		);
	}

	return result;
}

bool Cuboid::IsSmall() const noexcept
{
	return (std::abs(x.first) <= 50) && (std::abs(x.second) <= 50) && (std::abs(y.first) <= 50) &&
	       (std::abs(y.second) <= 50) && (std::abs(z.first) <= 50) && (std::abs(z.second) <= 50);
}

std::uint64_t Cuboid::Count() const noexcept
{
	return (x.second - x.first + 1) * (y.second - y.first + 1) * (z.second - z.first + 1);
}
