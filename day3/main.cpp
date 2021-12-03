
#include "shared.hpp"
#include <iostream>
#include <functional>
#include <chrono>
#include <bitset>
#include <array>

template<size_t N>
std::vector<std::bitset<N>> LinesToBitsets(const std::vector<std::string> &lines);

template<size_t N>
std::uint32_t SolvePart1(const std::vector<std::bitset<N>> &input);

template<size_t N>
std::uint32_t SolvePart2(const std::vector<std::bitset<N>> &input);

template<size_t N>
std::uint32_t FindRating(const std::vector<std::bitset<N>> &input,
                         const std::function<std::uint8_t(
		                         const std::vector<std::bitset<N>> &input, std::uint8_t position
                         )> &criterion);

template<size_t N>
std::uint8_t FindMostCommonBit(const std::vector<std::bitset<N>> &input, std::uint8_t position);

template<size_t N>
std::uint8_t FindLeastCommonBit(const std::vector<std::bitset<N>> &input, std::uint8_t position);

constexpr auto BitWidth = 12;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	const auto linesBits = LinesToBitsets<BitWidth>(lines);

	const auto beginSolving = std::chrono::steady_clock::now();
	const auto part1Result = SolvePart1(linesBits);
	const auto part2Result = SolvePart2(linesBits);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

template<size_t N>
std::vector<std::bitset<N>> LinesToBitsets(const std::vector<std::string> &lines)
{
	std::vector<std::bitset<N>> result;
	result.reserve(lines.size());
	for (const auto &line: lines)
	{
		if (line.length() != N)
		{
			throw std::runtime_error("Invalid input");
		}
		const auto integer = StrToInteger<std::uint32_t>(line, 2);
		result.emplace_back(integer);
	}
	return result;
}

template<size_t N>
std::uint32_t SolvePart1(const std::vector<std::bitset<N>> &input)
{
	std::array<std::int16_t, N> counters{};
	for (const auto &bits: input)
	{
		for (auto i = 0U; i < N; ++i)
		{
			if (bits.test(i))
			{
				++counters[i];
			}
			else
			{
				--counters[i];
			}
		}
	}
	std::bitset<N> gamma{};
	for (auto i = 0U; i < N; ++i)
	{
		if (counters.at(i) > 0)
		{
			gamma.set(i);
		}
	}
	auto epsilon = gamma;
	epsilon.flip();

	return gamma.to_ulong() * epsilon.to_ulong();
}

template<size_t N>
std::uint32_t SolvePart2(const std::vector<std::bitset<N>> &input)
{
	const auto generator = FindRating(input, FindMostCommonBit<N>);
	const auto scrubber = FindRating(input, FindLeastCommonBit<N>);
	return generator * scrubber;
}

template<size_t N>
std::uint32_t FindRating(const std::vector<std::bitset<N>> &input,
                         const std::function<std::uint8_t(
		                         const std::vector<std::bitset<N>> &input, std::uint8_t position
                         )> &criterion)
{
	std::vector<std::bitset<N>> data = input;
	std::uint8_t position = N - 1;
	while (data.size() > 1)
	{
		const auto keepBit = criterion(data, position);
		auto it = std::remove_if(data.begin(), data.end(),
		                         [keepBit, position](const std::bitset<N> &bits)
		                         {
			                         return bits[position] != static_cast<bool>(keepBit);
		                         });
		data.erase(it, data.end());
		position--;
	}
	return data.front().to_ulong();
}

template<size_t N>
std::uint8_t FindMostCommonBit(const std::vector<std::bitset<N>> &input, std::uint8_t position)
{
	std::int16_t counter{};
	for (const auto &bits: input)
	{
		if (bits.test(position))
		{
			++counter;
		}
		else
		{
			--counter;
		}
	}

	return counter >= 0;
}

template<size_t N>
std::uint8_t FindLeastCommonBit(const std::vector<std::bitset<N>> &input, std::uint8_t position)
{
	return !FindMostCommonBit(input, position);
}

