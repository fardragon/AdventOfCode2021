
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <array>
#include <numeric>
#include <vector>


std::vector<std::uint8_t> ParseInput(const std::vector<std::string> &lines);

std::uint64_t RunSimulation(const std::vector<std::uint8_t> &seed, std::uint16_t days);

std::uint64_t SolvePart1(const std::vector<std::uint8_t> &seed);

std::uint64_t SolvePart2(const std::vector<std::uint8_t> &seed);

using State = std::array<std::uint64_t, 9>;

State SimulateDay(const State &currentState);


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

std::vector<std::uint8_t> ParseInput(const std::vector<std::string> &lines)
{
	if (lines.size() != 1)
	{
		throw std::runtime_error("Failed to parse input");
	}
	std::vector<std::uint8_t> result{};
	const auto lineSplit = SplitString(lines.front(), ',');

	result.reserve(lineSplit.size());
	for (const auto &elem: lineSplit)
	{
		result.push_back(StrToInteger<std::uint8_t>(elem));
	}
	return result;
}


std::uint64_t SolvePart1(const std::vector<std::uint8_t> &seed)
{
	return RunSimulation(seed, 80);
}

std::uint64_t SolvePart2(const std::vector<std::uint8_t> &seed)
{
	return RunSimulation(seed, 256);
}

State SimulateDay(const State &currentState)
{
	State newState{};
	for (auto i = 0U; i < newState.size(); ++i)
	{
		if (i == 8)
		{
			newState[i] = currentState[0];
		}
		else if (i == 6)
		{
			newState[i] = currentState[0] + currentState[i + 1];
		}
		else
		{
			newState[i] = currentState[i + 1];
		}
	}
	return newState;
}

std::uint64_t RunSimulation(const std::vector<std::uint8_t> &seed, std::uint16_t days)
{
	State state{};
	for (const auto &fish: seed)
	{
		state[fish] += 1;
	}

	for (auto i = 0U; i < days; ++i)
	{
		state = SimulateDay(state);
	}

	return std::accumulate(state.begin(), state.end(), 0UL);
}
