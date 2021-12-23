
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <utility>
#include "Burrow.hpp"


std::array<std::pair<char, char>, 4> ParseInput(const std::vector<std::string> &lines);
std::uint64_t SolvePart1(const std::array<std::pair<char, char>, 4> &input);
std::uint64_t SolvePart2(const std::array<std::pair<char, char>, 4> &input);

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

std::array<std::pair<char, char>, 4> ParseInput(const std::vector<std::string> &lines)
{

	const auto topElements = SplitString(lines[2], '#');
	const auto bottomElements = SplitString(SplitStringWhitespace(lines[3])[0], '#');

	return {
			std::make_pair(topElements[3].front(), bottomElements[1].front()),
			std::make_pair(topElements[4].front(), bottomElements[2].front()),
			std::make_pair(topElements[5].front(), bottomElements[3].front()),
			std::make_pair(topElements[6].front(), bottomElements[4].front())
	};
}

std::uint64_t SolvePart1(const std::array<std::pair<char, char>, 4> &input)
{
	std::array<std::array<char, 2>, 4> initialState {};

	for (auto i = 0U; i < 4; ++i)
	{
		initialState[i][0] = input[i].first;
		initialState[i][1] = input[i].second;
	}

	Burrow<2> burrow(initialState);
	return burrow.Solve();
}

std::uint64_t SolvePart2(const std::array<std::pair<char, char>, 4> &input)
{
	std::array<std::array<char, 4>, 4> initialState {};

	const std::array<char, 4> extra1 = {'D', 'C', 'B', 'A'};
	const std::array<char, 4> extra2 = {'D', 'B', 'A', 'C'};

	for (auto i = 0U; i < 4; ++i)
	{
		initialState[i][0] = input[i].first;
		initialState[i][1] = extra1[i];
		initialState[i][2] = extra2[i];
		initialState[i][3] = input[i].second;
	}

	Burrow<4> burrow(initialState);
	return burrow.Solve();
}