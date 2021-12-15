
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include "Graph.hpp"



using Cave = std::vector<std::vector<uint8_t>>;
Cave ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const Cave &cave);
std::uint64_t SolvePart2(Cave cave);

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

Cave ParseInput(const std::vector<std::string> &lines)
{

	Cave result {};
	result.reserve(lines.size());

	const auto width = lines[0].size();
	for (const auto &line: lines)
	{
		if (line.length() != width)
		{
			throw std::runtime_error("Failed to parse input");
		}
		std::vector<std::uint8_t> row{};
		row.reserve(width);

		for (const auto &c: line)
		{
			row.push_back(c - '0');
		}
		result.push_back(std::move(row));
	}


	return result;
}

std::uint64_t SolvePart1(const Cave &cave)
{
	Graph graph(cave);
	return graph.ShortestPath();
}

std::uint64_t SolvePart2(Cave cave)
{
	// expand to the right
	for (auto & y : cave)
	{
		y.reserve(y.size() * 5);
		auto line =  y;
		for (auto i = 0U; i < 4U; ++i)
		{
			for (auto &elem : line)
			{
				if (++elem == 10)
				{
					elem = 1;
				}
			}
			std::copy(line.begin(), line.end(), std::back_inserter(y));
		}
	}

	// expand to the bottom
	auto caveCopy = cave;
	cave.reserve(cave.size() * 5);
	for (auto i = 0U; i < 4U; ++i)
	{
		for (auto &line: caveCopy)
		{
			for (auto &elem : line)
			{
				if (++elem == 10)
				{
					elem = 1;
				}
			}
			cave.push_back(line);
		}
	}

	Graph graph(cave);
	return graph.ShortestPath();
}


