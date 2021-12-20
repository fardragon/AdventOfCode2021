
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>

#include "Image.hpp"


std::pair<Algorithm, Image> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const Algorithm &algo, Image image);
std::uint64_t SolvePart2(const Algorithm &algo, Image image);

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


std::pair<Algorithm, Image> ParseInput(const std::vector<std::string> &lines)
{
	if (lines.front().size() != 512)
	{
		throw std::runtime_error("Failed to parse input");
	}


	Algorithm algorithm {};
	for (auto i = 0U; i < 512; ++i)
	{
		if (lines.front()[i] == '#')
		{
			algorithm[i] = true;
		}
	}

	if (!lines[1].empty())
	{
		throw std::runtime_error("Failed to parse input");
	}

	const std::size_t width = lines[2].length();
	std::vector<bool> pixels;

	for (auto it = std::next(lines.begin(), 2); it != lines.end(); ++it)
	{
		if (it->length() != width)
		{
			throw std::runtime_error("Failed to parse input");
		}
		for(const auto &c: *it)
		{
			pixels.push_back(c == '#');
		}
	}

	const auto height = pixels.size() / width;

	return std::make_pair(algorithm, Image(std::move(pixels), width, height));
}

std::uint64_t SolvePart1(const Algorithm &algo, Image image)
{
	image.Pad(10);

	for (auto i = 0U;  i < 2U; ++i)
	{
		image.Enhance(algo);
	}

	image.Strip(8);
	return image.Count();
}

std::uint64_t SolvePart2(const Algorithm &algo, Image image)
{
	image.Pad(150);

	for (auto i = 0U;  i < 50U; ++i)
	{
		image.Enhance(algo);
	}


	image.Strip(50);
	return image.Count();
}
