
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "Cave.hpp"


using CaveSystem = std::unordered_map<std::string, Cave>;
using Path = std::vector<std::string>;

CaveSystem ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const CaveSystem &caveSystem);
std::uint64_t SolvePart2(const CaveSystem &caveSystem);


void
DFS(const std::string &currentCave, const std::string &destinationCave, std::unordered_multiset<std::string> &visitedCaves,
    Path &currentPath, const CaveSystem &caveSystem, std::vector<Path> &foundPaths, bool allowRevisit);

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


CaveSystem ParseInput(const std::vector<std::string> &lines)
{
	CaveSystem caveSystem;

	const auto AddCave = [](CaveSystem &caveSystem,
	                        const std::string &name)
	{
		if (!caveSystem.contains(name))
		{
			caveSystem.emplace(
					name,
					Cave{name, {}}
			);
		}
		return std::ref(caveSystem[name]);
	};

	for (const auto &line: lines)
	{
		const auto ends = SplitString(line, '-');
		if (ends.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}
		auto end1 = AddCave(caveSystem, ends[0]);
		auto end2 = AddCave(caveSystem, ends[1]);
		end1.get().connections.push_back((ends[1]));
		end2.get().connections.push_back((ends[0]));
	}
	return caveSystem;
}

std::uint64_t SolvePart1(const CaveSystem &caveSystem)
{
	if (!caveSystem.contains("start") || !caveSystem.contains("end"))
	{
		throw std::runtime_error("Failed to solve part 1");
	}

	std::unordered_multiset<std::string> visited{};
	Path currentPath;
	std::vector<Path> foundPaths;

	DFS("start", "end", visited, currentPath, caveSystem, foundPaths, false);

	return foundPaths.size();
}

std::uint64_t SolvePart2(const CaveSystem &caveSystem)
{
	if (!caveSystem.contains("start") || !caveSystem.contains("end"))
	{
		throw std::runtime_error("Failed to solve part 1");
	}

	std::unordered_multiset<std::string> visited{};
	Path currentPath;
	std::vector<Path> foundPaths;

	DFS("start", "end", visited, currentPath, caveSystem, foundPaths, true);

	return foundPaths.size();
}

void
DFS(const std::string &currentCave, const std::string &destinationCave, std::unordered_multiset<std::string> &visitedCaves,
    Path &currentPath, const CaveSystem &caveSystem, std::vector<Path> &foundPaths, bool allowRevisit)
{
	const auto &cave = caveSystem.at(currentCave);
	if (!cave.IsBig())
	{
		visitedCaves.emplace(cave.name);
	}

	currentPath.push_back(cave.name);

#ifndef NDEBUG
	std::cout << "Current path: ";
	for (const auto &elem: currentPath)
	{
		std::cout << elem << ",";
	}
	std::cout << "\r\n";
#endif

	if (currentCave == destinationCave)
	{
		foundPaths.push_back(currentPath);
	}
	else
	{
		for (const auto &neighbour: cave.connections)
		{
			if (visitedCaves.contains(neighbour))
			{
				if (allowRevisit && neighbour != "start")
				{
					DFS(neighbour, destinationCave, visitedCaves, currentPath, caveSystem, foundPaths, false);
				}
			}
			else
			{
				DFS(neighbour, destinationCave, visitedCaves, currentPath, caveSystem, foundPaths, allowRevisit);
			}
		}
	}
	auto hit = visitedCaves.find(cave.name);
	if (hit != visitedCaves.end())
	{
		visitedCaves.erase(hit);
	}
	currentPath.pop_back();
}
