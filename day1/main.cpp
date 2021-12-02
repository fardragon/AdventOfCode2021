
#include "shared.hpp"
#include <iostream>
#include <numeric>
#include <chrono>

std::uint16_t SolvePart1(const std::vector<uint16_t> &input);
std::uint16_t SolvePart2(const std::vector<uint16_t> &input);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	const auto input = LinesToUint16(lines);

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


std::uint16_t SolvePart1(const std::vector<uint16_t> &input)
{
	std::uint16_t result{};

	for (auto it = input.begin(); it != std::prev(input.end(), 1); ++it)
	{
		if (*(it) < *(std::next(it, 1)))
		{
			++result;
		}
	}

	return result;
}

std::uint16_t SolvePart2(const std::vector<uint16_t> &input)
{
	std::uint16_t result{};

	std::uint16_t currentWindow = std::accumulate(input.begin(), input.begin() + 3, 0);
	for (auto it = std::next(input.begin(), 1); it != std::prev(input.end(), 2); ++it)
	{
		std::uint16_t nextWindow = std::accumulate(it, it + 3, 0);
		if (nextWindow > currentWindow)
		{
			++result;
		}
		currentWindow = nextWindow;
	}

	return result;
}
