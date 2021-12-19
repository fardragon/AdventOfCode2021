#ifndef ADVENTOFCODE2021_BEACON_HPP
#define ADVENTOFCODE2021_BEACON_HPP

#include <cstdint>
#include <optional>
#include <tuple>


using Offset = std::tuple<std::int32_t, std::int32_t, std::int32_t>;

struct Beacon
{
	std::int32_t x, y, z;

	bool operator<=>(const Beacon &other) const = default;
	Beacon operator-(const Beacon &other) const;
	Beacon operator+(const Beacon &other) const;
};


#endif //ADVENTOFCODE2021_BEACON_HPP
