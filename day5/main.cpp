
#include "shared.hpp"
#include "Line.hpp"
#include "Map.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iterator>


std::vector<Line> ParseInput(const std::vector<std::string> &input);
std::size_t FindMapSize(const std::vector<Line> &input);

std::size_t SolvePart1(const std::vector<Line> &input);
std::size_t SolvePart2(const std::vector<Line> &input);

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

std::vector<Line> ParseInput(const std::vector<std::string> &input)
{
	const auto parsePoint = [](const std::string &pointStr)
	{
		const auto pointSplit = SplitString(pointStr, ',');
		if (pointSplit.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}
		return std::make_pair(StrToInteger<std::uint16_t>(pointSplit[0]), StrToInteger<std::uint16_t>(pointSplit[1]));
	};

	std::vector<Line> result;
	result.reserve(input.size());
	for (const auto &line: input)
	{
		const auto lineSplit = SplitStringWhitespace(line);
		if (lineSplit.size() != 3)
		{
			throw std::runtime_error("Failed to parse input");
		}

		result.emplace_back(parsePoint(lineSplit[0]), parsePoint(lineSplit[2]));
	}
	return result;
}


std::size_t SolvePart1(const std::vector<Line> &input)
{
	const auto mapSize = FindMapSize(input);
	auto map = Map{mapSize};
	for (const auto &line: input)
	{
		if (line.IsHorizontal() || line.IsVertical())
		{
			map.DrawLine(line);
		}
	}
//	std::cout << map.DrawMap();
	return map.CountIntersections();
}

std::size_t SolvePart2(const std::vector<Line> &input)
{
	const auto mapSize = FindMapSize(input);
	auto map = Map{mapSize};
	for (const auto &line: input)
	{
		map.DrawLine(line);
	}
//	std::cout << map.DrawMap();
	return map.CountIntersections();
}

std::size_t FindMapSize(const std::vector<Line> &input)
{
	std::size_t size {};
	for (const auto &line: input)
	{
		const auto points = line.GetPoints();
		for(const auto &point: points)
		{
			if (point.first > size)
			{
				size = point.first;
			}

			if (point.second > size)
			{
				size = point.second;
			}
		}
	}
	return size;
}
