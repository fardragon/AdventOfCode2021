
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <unordered_map>

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>>
ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &entries);
std::uint64_t SolvePart2(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &entries);

std::unordered_map<std::string, std::uint8_t> DetermineEntryMapping(std::vector<std::string> signals);


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

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>>
ParseInput(const std::vector<std::string> &lines)
{
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> result{};
	result.reserve(lines.size());
	for (const auto &line: lines)
	{
		const auto splitPipe = SplitString(line, '|');
		if (splitPipe.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}

		auto patterns = SplitStringWhitespace(splitPipe[0]);
		auto outputs = SplitStringWhitespace(splitPipe[1]);

		for (auto &pattern : patterns)
		{
			std::sort(pattern.begin(), pattern.end());
		}

		for (auto &output : outputs)
		{
			std::sort(output.begin(), output.end());
		}

		result.emplace_back(std::move(patterns), std::move(outputs));
	}

	return result;
}

std::uint64_t SolvePart1(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &entries)
{
	std::uint64_t result{};
	for (const auto &entry: entries)
	{
		for (const auto &output: entry.second)
		{
			switch (output.length())
			{
				case 2:
				case 4:
				case 3:
				case 7:
					result += 1;
					break;
				default:
					break;
			}
		}
	}

	return result;
}

std::uint64_t SolvePart2(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> &entries)
{
	std::uint64_t result {};
	for (const auto &entry: entries)
	{
		std::uint64_t entryResult {};
		const auto mapping = DetermineEntryMapping(entry.first);
		for (const auto &digit: entry.second)
		{
			entryResult = entryResult * 10 + mapping.at(digit);
		}
		result += entryResult;
	}

	return result;
}

std::unordered_map<std::string, std::uint8_t> DetermineEntryMapping(std::vector<std::string> signals)
{
	std::unordered_map<std::string, std::uint8_t> result{};
	result.reserve(10);

	// determine by length
	const auto findByLength = [] (std::vector<std::string> &signals, std::uint8_t length)
	{
		auto it = std::find_if(signals.begin(), signals.end(), [length](const auto &signal) {return signal.length() == length; });
		if (it == signals.end())
		{
			throw std::runtime_error("Failed to determine by length");
		}
		auto result = *it;
		signals.erase(it);
		return result;
	};

	const auto extractByLength = [] (std::vector<std::string> &signals, std::uint8_t length)
	{
		auto it = std::partition(signals.begin(), signals.end(), [length](const auto &signal) {return signal.length() != length; });
		if (it == signals.end())
		{
			throw std::runtime_error("Failed to extract by length");
		}

		std::vector<std::string> result{std::make_move_iterator(it), std::make_move_iterator(signals.end())};
		signals.erase(it, signals.end());

		return result;
	};

	const auto findByIntersection = [] (std::vector<std::string> &signals, const std::string &target, std::uint8_t targetIntersection)
	{
		for (auto it = signals.begin(); it != signals.end(); ++it)
		{
			std::uint8_t sameChars {};
			for (const auto &c: target)
			{
				if (it->find(c) != it->npos)
				{
					++sameChars;
				}
			}
			if (sameChars == targetIntersection)
			{
				auto result = *it;
				signals.erase(it);
				return result;
			}
		}
		throw std::runtime_error("Failed to determine by intersection");
	};


	// find 1

		auto one = findByLength(signals, 2);
		result[one] = 1;


	// find 4

		auto four = findByLength(signals, 4);
		result[four] = 4;

	// four left (four without vertical line)
		std::string fourLeft{};
		std::copy_if(four.begin(), four.end(), std::back_inserter(fourLeft),
					 [&one] (const char &c)
					 {
						return one.find(c) == std::string::npos;
					 });


	// find 7
	{
		auto seven = findByLength(signals, 3);
		result[seven] = 7;
	}


	// find 8
	{
		auto eighth = findByLength(signals, 7);
		result[eighth] = 8;
	}

	// determine 5 segment digits (2, 3, 5)
	{
		auto candidates = extractByLength(signals, 5);
		if (candidates.size() != 3)
		{
			throw std::runtime_error("Failed to solve part 3");
		}

		// 5 has 2 common segments with four left
		auto five = findByIntersection(candidates, fourLeft, 2);
		result[five] = 5;

		// 3 has 2 common segments with 1
		auto three = findByIntersection(candidates, one, 2);
		result[three] = 3;

		// remaining digit is 2
		result[candidates.front()] = 2;
	}

	// determine remaining (0, 6, 9)
	{
		if (signals.size() != 3)
		{
			throw std::runtime_error("Failed to solve part 3");
		}

		// 9 has 4 common segments with 4
		auto nine = findByIntersection(signals, four, 4);
		result[nine] = 9;

		// 6 has 2 common segments with four left
		auto six = findByIntersection(signals, fourLeft, 2);
		result[six] = 6;

		// remaining digit is 0
		result[signals.front()] = 0;
	}



	return result;
}
