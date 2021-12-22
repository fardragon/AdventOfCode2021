
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include "Cuboid.hpp"


using Rule = std::pair<Cuboid, bool>;

std::vector<Rule> ParseInput(const std::vector<std::string> &lines);
std::uint64_t SolvePart1(const std::vector<Rule> &rules);
std::uint64_t SolvePart2(const std::vector<Rule> &rules);

std::pair<std::int64_t, std::int64_t> ParseCoords(const std::string &str);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	[[maybe_unused]] const auto input = ParseInput(lines);

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

std::vector<Rule> ParseInput(const std::vector<std::string> &lines)
{
	std::vector<Rule> result;
	result.reserve(lines.size());

	for (const auto &line: lines)
	{
		const auto elems = SplitStringWhitespace(line);
		const auto coords = SplitString(elems[1], ',');

		auto command = elems[0] == "on";

		const auto x = ParseCoords(coords[0]);
		const auto y = ParseCoords(coords[1]);
		const auto z = ParseCoords(coords[2]);

		result.emplace_back(Cuboid{x, y, z}, command);
	}

	return result;
}

std::pair<std::int64_t, std::int64_t> ParseCoords(const std::string &str)
{
	const auto split1 = SplitString(str, '=');
	const auto split2 = SplitString(split1[1], '.');

	return {StrToInteger<std::int64_t>(split2[0]), StrToInteger<std::int64_t>(split2[2])};
}

std::uint64_t SolvePart1(const std::vector<Rule> &rules)
{
	std::vector<Cuboid> active;

	for(const auto &rule: rules)
	{
		if (!rule.first.IsSmall()) continue;

		std::vector<Cuboid> newActive;
		for (const auto &cub : active)
		{
			auto intersection = cub.RemoveIntersection(rule.first);
			std::copy(intersection.begin(), intersection.end(), std::back_inserter(newActive));
		}
		if (rule.second)
		{
			newActive.push_back(rule.first);
		}
		active = newActive;
	}

	std::uint64_t result {};
	for (const auto &cub: active)
	{
		result += cub.Count();
	}
	return result;
}

std::uint64_t SolvePart2(const std::vector<Rule> &rules)
{
	std::vector<Cuboid> active;

	for(const auto &rule: rules)
	{
		std::vector<Cuboid> newActive;
		for (const auto &cub : active)
		{
			auto intersection = cub.RemoveIntersection(rule.first);
			std::copy(intersection.begin(), intersection.end(), std::back_inserter(newActive));
		}
		if (rule.second)
		{
			newActive.push_back(rule.first);
		}
		active = newActive;
	}

	std::uint64_t result {};
	for (const auto &cub: active)
	{
		result += cub.Count();
	}
	return result;
}