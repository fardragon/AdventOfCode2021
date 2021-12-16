
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <charconv>
#include <bitset>
#include "Packet.hpp"
#include "Parser.hpp"




Packet ParseInput(const std::vector<std::string> &lines);
std::vector<std::uint8_t> ParseHexadecimalString(std::string_view str);

std::uint64_t SolvePart1(const Packet &packet);
std::uint64_t SolvePart2(const Packet &packet);


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


Packet ParseInput(const std::vector<std::string> &lines)
{
	if (lines.size() != 1)
	{
		throw std::runtime_error("Failed to parse input");
	}
	const auto bits = ParseHexadecimalString(lines.front());

	Parser parser{};

	return parser.ParsePackets(bits);
}

std::vector<std::uint8_t> ParseHexadecimalString(std::string_view str)
{
	std::vector<std::uint8_t> result {};
	result.reserve(str.size() * 4);

	for (const auto &hex: str)
	{
		std::uint8_t nibble {};
		const auto convResult = std::from_chars(&hex, &hex + 1, nibble, 16);
		if (convResult.ec != std::errc())
		{
			throw std::runtime_error("Failed to parse input");
		}
		std::bitset<4> bits{nibble};
		for(auto i = 0U; i < bits.size(); ++i)
		{
			result.push_back(bits[bits.size() - 1 -i] ? 1 : 0);
		}
	}

	return result;
}


std::uint64_t SolvePart1(const Packet &packet)
{
	return packet.GetVersionSum();
}

std::uint64_t SolvePart2(const Packet &packet)
{
	return packet.Solve();
}