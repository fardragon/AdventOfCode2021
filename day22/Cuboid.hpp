//
// Created by fardragon on 12/22/21.
//

#ifndef ADVENTOFCODE2021_CUBOID_HPP
#define ADVENTOFCODE2021_CUBOID_HPP


#include <utility>
#include <cstdint>
#include <optional>
#include <vector>

class Cuboid
{
public:
	Cuboid(std::pair<std::int64_t, std::int64_t> x, std::pair<std::int64_t, std::int64_t> y,
	       std::pair<std::int64_t, std::int64_t> z);


	[[nodiscard]] std::vector<Cuboid> RemoveIntersection(const Cuboid &other) const noexcept;
	[[nodiscard]] bool IsSmall() const noexcept;
	[[nodiscard]] std::uint64_t Count() const noexcept;

	std::strong_ordering operator<=>(const Cuboid &) const noexcept = default;
private:
	[[nodiscard]] std::vector<Cuboid> Remove(const Cuboid &other) const noexcept;
	[[nodiscard]] std::optional<Cuboid> Intersection(const Cuboid &other) const noexcept;
	[[nodiscard]] bool Intersects(const Cuboid &other) const noexcept;

private:
	std::pair<std::int64_t, std::int64_t> x;
	std::pair<std::int64_t, std::int64_t> y;
	std::pair<std::int64_t, std::int64_t> z;
};


#endif //ADVENTOFCODE2021_CUBOID_HPP
