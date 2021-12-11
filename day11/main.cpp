
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <array>
#include <deque>
#include <unordered_set>

constexpr std::int64_t N = 10;
using OctopusGrid = std::array<std::uint8_t, N * N>;
using Position = std::pair<std::int64_t, std::int64_t>;


OctopusGrid ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(OctopusGrid heightmap);
std::uint64_t SolvePart2(OctopusGrid heightmap);

std::uint64_t SimulateStep(OctopusGrid &grid);

std::vector<Position> GetNeighbours(const Position &currentPosition);

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
	const auto part1Result = SolvePart1(input);
	const auto part2Result = SolvePart2(input);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

OctopusGrid ParseInput(const std::vector<std::string> &lines)
{
	if(lines.size() != N)
	{
		throw std::runtime_error("Failed to parse input");
	}
	OctopusGrid result {};
	auto it = result.begin();

	for (const auto &line: lines)
	{
		if (line.length() != N)
		{
			throw std::runtime_error("Failed to parse input");
		}

		for (const auto &c: line)
		{
			*(it++) = (c - '0');
		}
	}

	return result;
}

std::uint64_t SolvePart1(OctopusGrid heightmap)
{
	std::uint64_t result {};
	for (auto i = 1U; i <= 100U; ++i)
	{
		result += SimulateStep(heightmap);
	}
	return result;
}

std::uint64_t SolvePart2(OctopusGrid heightmap)
{
	for (auto i = 1U; i <= 1000U; ++i)
	{
		if (SimulateStep(heightmap) == N*N)
		{
			return i;
		}
	}
	throw std::runtime_error("Failed to solve part 2");
}


std::uint64_t SimulateStep(OctopusGrid &grid)
{
	std::deque<Position> positionsToFlash {};
	std::unordered_set<std::size_t> flashedPositions {};

	for (auto x = 0U; x < N; ++x)
	{
		for (auto y = 0U; y < N; ++y)
		{
			auto index = y * N + x;
			grid[index] += 1;
			if (grid[index] > 9)
			{
				// flash in this step
				positionsToFlash.emplace_back(x, y);
			}
		}
	}

	std::uint64_t flashes {};
	while (!positionsToFlash.empty())
	{
		const auto &position = positionsToFlash.front();
		const auto index = position.second * N + position.first;

		if (!flashedPositions.contains(index))
		{
			grid[index] = 0;
			flashes += 1;
			flashedPositions.emplace(index);

			const auto neighbours = GetNeighbours(position);

			for (const auto &neighbour: neighbours)
			{
				auto neighbourIndex = neighbour.second * N + neighbour.first;

				if (!flashedPositions.contains(neighbourIndex))
				{
					// not flashed yet;
					grid[neighbourIndex] += 1;
					if (grid[neighbourIndex] > 9)
					{
						positionsToFlash.push_back(neighbour);
					}
				}
			}
		}
		positionsToFlash.pop_front();
	}
	return flashes;
}

std::vector<Position> GetNeighbours(const Position &currentPosition)
{
	static const std::array<Position, 8> steps =
			{
					std::make_pair(-1, -1),
					std::make_pair(-1, 0),
					std::make_pair(-1, 1),
					std::make_pair(0, -1),
					std::make_pair(0, 1),
					std::make_pair(1, -1),
					std::make_pair(1, 0),
					std::make_pair(1, 1),
			};

	std::vector<Position> result {};

	for (const auto &step : steps)
	{
		auto candidate = Position(currentPosition.first + step.first, currentPosition.second + step.second);

		if ((candidate.first >= 0)
			&& (candidate.first < N)
			&& (candidate.second >= 0)
			&& (candidate.second < N)
			)
		{
			result.push_back(candidate);
		}


	}

	return result;
}


