
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <unordered_map>

std::tuple<std::size_t, std::size_t, std::vector<std::uint8_t>> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap);

std::uint64_t SolvePart2(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap);

std::vector<std::pair<std::size_t, std::size_t>>
GetNeighbours(std::size_t x, std::size_t y, std::size_t width, std::size_t height);

std::uint64_t CalculateBasinSize(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap,
                                 std::pair<std::size_t, std::size_t> currentPoint);


std::uint64_t TraverseBasin(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap,
                            std::pair<std::size_t, std::size_t> currentPoint, std::vector<bool> &heightmapVisited);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	const auto input = ParseInput(lines);

	const auto beginSolving = std::chrono::steady_clock::now();
	const auto part1Result = SolvePart1(std::get<0>(input), std::get<1>(input), std::get<2>(input));
	const auto part2Result = SolvePart2(std::get<0>(input), std::get<1>(input), std::get<2>(input));

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::tuple<std::size_t, std::size_t, std::vector<std::uint8_t>> ParseInput(const std::vector<std::string> &lines)
{
	const std::size_t width = lines.front().length();
	const std::size_t height = lines.size();

	std::vector<std::uint8_t> heightmap{};
	heightmap.reserve(width * height);

	for (const auto &line: lines)
	{
		if (line.length() != width)
		{
			throw std::runtime_error("Failed to parse input");
		}

		for (const auto &c: line)
		{
			heightmap.push_back(c - '0');
		}
	}

	return std::make_tuple(width, height, std::move(heightmap));
}

std::uint64_t SolvePart1(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap)
{
	std::uint64_t result{};
	for (auto x = 0U; x < width; ++x)
	{
		for (auto y = 0U; y < height; ++y)
		{
			const auto index = y * width + x;
			const auto neighbours = GetNeighbours(x, y, width, height);
			auto lowPoint = true;
			for (const auto &neighbour: neighbours)
			{
				lowPoint &= (heightmap[index] < heightmap[neighbour.second * width + neighbour.first]);
			}

			if (lowPoint)
			{
				result += (heightmap[index] + 1);
			}

		}
	}

	return result;
}

std::vector<std::pair<std::size_t, std::size_t>>
GetNeighbours(std::size_t x, std::size_t y, std::size_t width, std::size_t height)
{
	std::vector<std::pair<std::size_t, std::size_t>> neighbours;

	if (x > 0)
	{
		neighbours.emplace_back(x - 1, y);
	}

	if (x < width - 1)
	{
		neighbours.emplace_back(x + 1, y);
	}

	if (y > 0)
	{
		neighbours.emplace_back(x, y - 1);
	}

	if (y < height - 1)
	{
		neighbours.emplace_back(x, y + 1);
	}

	return neighbours;
}

std::uint64_t SolvePart2(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap)
{
	std::vector<std::pair<std::size_t, std::size_t>> lowPoints;
	for (auto x = 0U; x < width; ++x)
	{
		for (auto y = 0U; y < height; ++y)
		{
			const auto index = y * width + x;
			const auto neighbours = GetNeighbours(x, y, width, height);
			auto lowPoint = true;
			for (const auto &neighbour: neighbours)
			{
				lowPoint &= (heightmap[index] < heightmap[neighbour.second * width + neighbour.first]);
			}

			if (lowPoint)
			{
				lowPoints.emplace_back(x, y);
			}
		}
	}

	std::vector<std::uint64_t> basins;
	basins.reserve(lowPoints.size());
	for (const auto &lowPoint: lowPoints)
	{
		basins.emplace_back(CalculateBasinSize(width, height, heightmap, lowPoint));
	}

	if (basins.size() < 3)
	{
		throw std::runtime_error("Failed to solve part 2");
	}

	std::sort(basins.begin(), basins.end(), std::greater<>());

	return basins[0] * basins[1] * basins[2];
}

std::uint64_t CalculateBasinSize(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap,
                                 std::pair<std::size_t, std::size_t> startingPoint)
{
	std::vector<bool> heightmapVisited(heightmap.size(), false);
	return TraverseBasin(width, height, heightmap, startingPoint, heightmapVisited);
}

std::uint64_t TraverseBasin(std::size_t width, std::size_t height, const std::vector<std::uint8_t> &heightmap,
                            std::pair<std::size_t, std::size_t> currentPoint, std::vector<bool> &heightmapVisited)
{
	const auto index = currentPoint.second * width + currentPoint.first;
	if (heightmapVisited[index])
	{
		return 0;
	}
	else
	{
		heightmapVisited[index] = true;
	}

	if (heightmap[index] == 9)
	{
		return 0;
	}

	std::uint64_t size{1};

	const auto neighbours = GetNeighbours(currentPoint.first, currentPoint.second, width, height);
	for (const auto &neighbour: neighbours)
	{
		size += TraverseBasin(width, height, heightmap, neighbour, heightmapVisited);
	}

	return size;
}
