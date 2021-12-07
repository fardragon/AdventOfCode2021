
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>

std::vector<std::uint16_t> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(std::vector<std::uint16_t> startingPositions);
std::uint64_t SolvePart2(std::vector<std::uint16_t> startingPositions);


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

std::vector<std::uint16_t> ParseInput(const std::vector<std::string> &lines)
{
	if (lines.size() != 1)
	{
		throw std::runtime_error("Failed to parse input");
	}
	std::vector<std::uint16_t> result{};
	const auto lineSplit = SplitString(lines.front(), ',');

	result.reserve(lineSplit.size());
	for (const auto &elem: lineSplit)
	{
		result.push_back(StrToInteger<std::uint16_t>(elem));
	}
	return result;
}


std::uint64_t SolvePart1(std::vector<std::uint16_t> startingPositions)
{
	std::sort(startingPositions.begin(), startingPositions.end());

	const auto findMedian = [](const std::vector<std::uint16_t> &vec)
	{
		if (vec.size() % 2 == 0)
		{
			return static_cast<std::uint16_t>((vec[vec.size() / 2 - 1] + vec[vec.size() / 2]) / 2);
		}
		else
		{
			return vec[vec.size() / 2];
		}
	};

	std::uint16_t median = findMedian(startingPositions);

	return std::transform_reduce(startingPositions.begin(), startingPositions.end(), 0,
	                             std::plus<>(),
	                             [median](const auto &elem)
	                             {
		                             return std::abs(elem - median);
	                             });
}

std::uint64_t SolvePart2(std::vector<std::uint16_t> startingPositions)
{
	// The result should be in the interval [mean - 0.5, mean + 0.5]. This comes from the minimum of the fuel usage function

	const auto findFuelUsage = [](const std::vector<std::uint16_t> &startingPositions, uint16_t desiredPosition)
	{
		std::uint64_t usage = {};
		for (const auto &position: startingPositions)
		{
			const auto diff = std::abs(desiredPosition - position);
			usage += (diff * diff + diff) / 2;
		}
		return usage;
	};

	const auto mean = std::accumulate(startingPositions.begin(), startingPositions.end(), 0UL) / startingPositions.size();

	return std::min({findFuelUsage(startingPositions, mean), findFuelUsage(startingPositions, mean + 1), findFuelUsage(startingPositions, mean - 1)});
}

