
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <set>


using Range = std::pair<std::int64_t, std::int64_t>;
using Target = std::pair<Range, Range>;

Target ParseInput(const std::vector<std::string> &lines);

std::int64_t SolvePart1(const Target &target);
std::uint64_t SolvePart2(const Target &target);

std::int64_t SimulateProveTrajectory(const Target &target, std::pair<std::int16_t, std::int16_t> velocity);
bool SimulateProveTrajectory2(const Target &target, std::pair<std::int16_t, std::int16_t> velocity);

Range ParseRange(const std::string &range);

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

Target ParseInput(const std::vector<std::string> &lines)
{
	if (lines.size() != 1)
	{
		throw std::runtime_error("Failed to parse input");
	}

	auto elems = SplitStringWhitespace(lines[0]);
	if (elems.size() != 4)
	{
		throw std::runtime_error("Failed to parse input");
	}

	return {ParseRange(elems[2]), ParseRange(elems[3])};
}

Range ParseRange(const std::string &range)
{
	auto elems = SplitString(range, '=');

	if (elems.size() != 2)
	{
		throw std::runtime_error("Failed to parse input");
	}

	auto bounds = SplitString(elems[1], '.');


	return Range(
			StrToInteger<std::int64_t>(bounds[0]),
			StrToInteger<std::int64_t>(bounds[2])
	);
}

std::int64_t SolvePart1(const Target &target)
{
	std::set<std::int64_t> candidates;

	for (auto x = -500; x <= 500; ++x)
	{
		for(auto y = -500; y <= 500; ++y)
		{
			const auto result = SimulateProveTrajectory(target, {x, y});
			if (result != 0)
			{
				candidates.emplace(result);
			}
		}
	}

	return *std::prev(candidates.end());
}

std::uint64_t SolvePart2(const Target &target)
{
	std::uint64_t result {};

	for (auto x = 0; x <= 500; ++x)
	{
		for(auto y = -500; y <= 500; ++y)
		{
			const auto hit = SimulateProveTrajectory2(target, {x, y});
			if (hit)
			{
				result += 1;
			}
		}
	}

	return result;
}

std::int64_t SimulateProveTrajectory(const Target &target, std::pair<std::int16_t, std::int16_t> velocity)
{
	std::int64_t maxY{0};

	std::int64_t xPos{0};
	std::int64_t yPos{0};

	while (true)
	{
		xPos += velocity.first;
		yPos += velocity.second;

		if (xPos > target.first.second)
		{
			return 0;
		}

		if (yPos < target.second.first)
		{
			return 0;
		}

		if (velocity.first != 0)
		{
			if (velocity.first > 0)
			{
				velocity.first -= 1;
			}
			else
			{
				velocity.first += 1;
			}
		}

		velocity.second -= 1;
		maxY = std::max(maxY, yPos);

		if ((xPos >= target.first.first)
		    && (xPos <= target.first.second)
		    && (yPos >= target.second.first)
		    && (yPos <= target.second.second)
				)
		{
			return maxY;
		}
	}
}

bool SimulateProveTrajectory2(const Target &target, std::pair<std::int16_t, std::int16_t> velocity)
{
	std::int64_t xPos{0};
	std::int64_t yPos{0};

	while (true)
	{
		xPos += velocity.first;
		yPos += velocity.second;

		if (xPos > target.first.second)
		{
			return false;
		}

		if (yPos < target.second.first)
		{
			return false;
		}

		if (velocity.first != 0)
		{
			if (velocity.first > 0)
			{
				velocity.first -= 1;
			}
			else
			{
				velocity.first += 1;
			}
		}

		velocity.second -= 1;

		if ((xPos >= target.first.first)
		    && (xPos <= target.first.second)
		    && (yPos >= target.second.first)
		    && (yPos <= target.second.second)
				)
		{
			return true;
		}
	}
}