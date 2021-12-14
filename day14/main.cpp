
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>



struct Polymer
{
	std::unordered_map<std::string, size_t> elements;
	char lastElement;
};

using Substituitions = std::unordered_map<std::string, char>;
std::pair<Polymer, Substituitions> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const Polymer &polymer, const Substituitions &substitutions);
std::uint64_t SolvePart2(const Polymer &polymer, const Substituitions &substitutions);

Polymer ApplySubstitutions(const Polymer &polymer, const Substituitions &substitutions);
std::uint64_t RunSubstitutions(Polymer polymer, const Substituitions &substitutions, std::size_t iterations);
std::unordered_map<char, std::size_t> CountPolymerElements(const Polymer &polymer);

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
	const auto part1Result = SolvePart1(input.first, input.second);
	const auto part2Result = SolvePart2(input.first, input.second);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::pair<Polymer, Substituitions> ParseInput(const std::vector<std::string> &lines)
{
	if ((lines.size() < 3) || (lines[0].empty()) || (!lines[1].empty()))
	{
		throw std::runtime_error("Failed to parse input");
	}

	Polymer polymer {{}, lines[0].back()};
	polymer.elements.reserve(lines[0].length() - 1);
	for(auto it = lines[0].begin(); it != std::prev(lines[0].end()); ++it)
	{
		std::string pair{*it, *(std::next(it))};
		polymer.elements[pair] += 1;
	}

	Substituitions instructions;
	instructions.reserve(lines.size() - 2);
	for (auto it = std::next(lines.begin(), 2); it != lines.end(); ++it)
	{
		const auto elems = SplitStringWhitespace(*it);
		if ((elems.size() != 3) || (elems[1] != "->") || (elems[0].size() != 2) || (elems[2].size() != 1))
		{
			throw std::runtime_error("Failed to parse input");
		}

		instructions[elems[0]] = elems[2].front();
	}

	return std::make_pair(std::move(polymer), std::move(instructions));
}

std::uint64_t SolvePart1(const Polymer &polymer, const Substituitions &substitutions)
{
	return RunSubstitutions(polymer, substitutions, 10);
}

std::uint64_t SolvePart2(const Polymer &polymer, const Substituitions &substitutions)
{
	return RunSubstitutions(polymer, substitutions, 40);
}

std::uint64_t RunSubstitutions(Polymer polymer, const Substituitions &substitutions, std::size_t iterations)
{
	for (auto i = 0U; i < iterations; ++i)
	{
		polymer = ApplySubstitutions(polymer, substitutions);
	}

	auto count = CountPolymerElements(polymer);


	std::vector<std::pair<char, size_t>> sortedCount{count.begin(), count.end()};

	std::sort(sortedCount.begin(), sortedCount.end(), []
			(const std::pair<char, size_t> &a, const std::pair<char, size_t> &b)
	{
		return a.second > b.second;
	});

	return sortedCount.front().second - sortedCount.back().second;
}


Polymer ApplySubstitutions(const Polymer &polymer, const Substituitions &substitutions)
{
	Polymer result {{}, polymer.lastElement};
	result.elements.reserve(polymer.elements.size());


	for (const auto  &[pair, count]: polymer.elements)
	{
		if (substitutions.contains(pair))
		{
			const std::string newPair1 = {pair.front(), substitutions.at(pair)};
			const std::string newPair2 = {substitutions.at(pair), pair.back()};

			result.elements[newPair1] += count;
			result.elements[newPair2] += count;
		}
		else
		{
			result.elements[pair] += count;
		}
	}

	return result;
}

std::unordered_map<char, std::size_t> CountPolymerElements(const Polymer &polymer)
{
	std::unordered_map<char, std::size_t> result;
	for (const auto &[pair, count]: polymer.elements)
	{
		result[pair.front()] += count;
	}
	result[polymer.lastElement] += 1;
	return result;
}

