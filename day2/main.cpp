
#include "shared.hpp"
#include <iostream>
#include <numeric>
#include <chrono>

std::uint32_t SolvePart1(const std::vector<std::pair<std::string, std::uint16_t>> &input);
std::uint32_t SolvePart2(const std::vector<std::pair<std::string, std::uint16_t>> &input);

//std::uint16_t SolvePart2(const std::vector<uint16_t> &input);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	const auto input = LinesToStrUint16(lines);

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

std::uint32_t SolvePart1(const std::vector<std::pair<std::string, std::uint16_t>> &input)
{
	std::uint32_t position{}, depth{};

	for (const auto &[command, argument]: input)
	{
		switch(command.front())
		{
			case 'f':
			{
				//forward
				position += argument;
				break;
			}
			case 'u':
			{
				// up
				depth -= argument;
				break;
			}
			case 'd':
			{
				// down
				depth += argument;
				break;
			}
			default:
				throw std::runtime_error("Invalid command");
		}
	}

	return position * depth;
}

std::uint32_t SolvePart2(const std::vector<std::pair<std::string, std::uint16_t>> &input)
{
	std::uint32_t position{}, depth{};
	std::int32_t aim{};

	for (const auto &[command, argument]: input)
	{
		switch(command.front())
		{
			case 'f':
			{
				//forward
				position += argument;
				depth += (aim * argument);
				break;
			}
			case 'u':
			{
				// up
				aim -= argument;
				break;
			}
			case 'd':
			{
				// down
				aim += argument;
				break;
			}
			default:
				throw std::runtime_error("Invalid command");
		}
	}

	return position * depth;
}