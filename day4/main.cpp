
#include "shared.hpp"
#include "BingoBaord.hpp"
#include <iostream>
#include <chrono>


std::pair<std::vector<std::uint8_t>, std::vector<Board>> ParseInput(const std::vector<std::string> &input);

std::vector<std::uint8_t> ParseNumbers(std::string_view numbers);

std::vector<std::uint8_t> ParseBoardElements(std::string_view elements);

std::uint32_t SolvePart1(const std::vector<std::uint8_t> &numbers, std::vector<Board> boards);

std::uint32_t SolvePart2(const std::vector<std::uint8_t> &numbers, std::vector<Board> boards);

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

std::pair<std::vector<std::uint8_t>, std::vector<Board>> ParseInput(const std::vector<std::string> &input)
{
	if (input.empty())
	{
		throw std::runtime_error("Failed to parse input");
	}
	auto cursor = input.begin();
	auto numbers = ParseNumbers(*cursor);
	++cursor;

	std::uint8_t boardNumber = 1;
	std::vector<Board> boards;
	while (cursor != input.end())
	{
		const auto remaining = std::distance(cursor, input.end());
		if (remaining < 6)
		{
			throw std::runtime_error("Failed to parse input");
		}

		if (!cursor->empty())
		{
			throw std::runtime_error("Failed to parse input");
		}

		std::advance(cursor, 1);
		std::string boardElements;
		for (auto it = cursor; it != cursor + 5; ++it)
		{
			boardElements.append(*it);
			boardElements.push_back(' ');
		}

		boards.emplace_back(boardNumber++, ParseBoardElements(boardElements));
		std::advance(cursor, 5);
	}


	return std::make_pair(std::move(numbers), std::move(boards));
}

std::vector<std::uint8_t> ParseNumbers(std::string_view numbers)
{
	auto splitNumbers = SplitString({numbers.data(), numbers.size()}, ',');
	std::vector<std::uint8_t> result{};
	result.reserve(splitNumbers.size());

	for (const auto &number: splitNumbers)
	{
		result.push_back(StrToInteger<std::uint8_t>(number));
	}
	return result;
}

std::vector<std::uint8_t> ParseBoardElements(std::string_view elements)
{
	std::vector<std::uint8_t> result{};
	std::vector<std::string> boardElements = SplitStringWhitespace({elements.data(), elements.size()});

	result.reserve(boardElements.size());
	for (const auto &element: boardElements)
	{
		result.push_back(StrToInteger<std::uint8_t>(element));
	}
	return result;
}

std::uint32_t SolvePart1(const std::vector<std::uint8_t> &numbers, std::vector<Board> boards)
{
	for (const auto &number: numbers)
	{
		for (auto &board: boards)
		{
			auto result = board.MarkNumber(number);
			if (result.has_value())
			{
				return result.value();
			}
		}
	}

	throw std::runtime_error("Failed to solve part 1");
}

std::uint32_t SolvePart2(const std::vector<std::uint8_t> &numbers, std::vector<Board> boards)
{
	for (const auto &number: numbers)
	{
		if (boards.empty()) break;
		std::vector<std::uint8_t> boardsToRemove;
		for (auto it = boards.begin(); it != boards.end(); ++it)
		{
			auto result = it->MarkNumber(number);
			if (result.has_value())
			{
				if (boards.size() == 1)
				{
					return result.value();
				}
				else
				{
					boardsToRemove.push_back(it->GetBoardNumber());
				}
			}
		}
		auto it = std::remove_if(boards.begin(), boards.end(),
		                         [&boardsToRemove](const Board &board)
		                         {
			                         return std::find(boardsToRemove.begin(), boardsToRemove.end(),
			                                          board.GetBoardNumber()) != boardsToRemove.end();
		                         }
		);
		boards.erase(it, boards.end());
	}

	throw std::runtime_error("Failed to solve part 2");
}
