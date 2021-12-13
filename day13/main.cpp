
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include <sstream>

enum class Cell
{
	Empty,
	Dot
};

enum class FoldDirection
{
	x,
	y
};

struct FoldInstruction
{
	FoldDirection direction;
	std::uint16_t line;
};

struct Origami
{
	std::vector<Cell> cells;
	std::uint16_t width;
	std::uint16_t height;
};

std::pair<Origami, std::vector<FoldInstruction>> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(const Origami &origami, const std::vector<FoldInstruction> &instructions);

std::string SolvePart2(Origami origami, const std::vector<FoldInstruction> &instructions);

Origami PerformFold(const Origami &origami, const FoldInstruction &instruction);


std::string StringifyOrigami(const Origami &origami, char emptyChar);


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
	std::cout << "Part 2 result: \r\n" << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::pair<Origami, std::vector<FoldInstruction>> ParseInput(const std::vector<std::string> &lines)
{
	auto sep = std::find_if(lines.begin(), lines.end(), [](const std::string &line)
	{ return line.empty(); });
	if (sep == lines.end())
	{
		throw std::runtime_error("Failed to parse input");
	}

	std::vector<std::pair<std::uint16_t, std::uint16_t>> dots{};
	dots.reserve(std::distance(lines.begin(), sep));

	std::uint16_t width{};
	std::uint16_t height{};


	for (auto it = lines.begin(); it != sep; ++it)
	{
		auto coords = SplitString(*it, ',');
		if (coords.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}

		dots.emplace_back(StrToInteger<std::int16_t>(coords[0]), StrToInteger<std::int16_t>(coords[1]));
		width = std::max(width, dots.back().first);
		height = std::max(height, dots.back().second);
	}
	width += 1; // account for 0 based indexes
	height += 1; // account for 0 based indexes

	Origami origami{
			{static_cast<std::size_t>(width * height), Cell::Empty},
			width,
			height
	};

	for (const auto &dot: dots)
	{
		origami.cells[dot.second * origami.width + dot.first] = Cell::Dot;
	}

	std::vector<FoldInstruction> instructions;
	instructions.reserve(std::distance(std::next(sep), lines.end()));
	for (auto it = std::next(sep); it != lines.end(); ++it)
	{
		auto words = SplitStringWhitespace(*it);
		if (words.size() != 3)
		{
			throw std::runtime_error("Failed to parse input");
		}

		auto elements = SplitString(words[2], '=');
		if (elements.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}

		FoldInstruction instruction{};

		switch (elements.front().front())
		{
			case 'y':
				instruction.direction = FoldDirection::y;
				break;
			case 'x':
				instruction.direction = FoldDirection::x;
				break;
		}
		instruction.line = StrToInteger<std::uint16_t>(elements[1]);

		instructions.push_back(instruction);
	}

	return {std::move(origami), std::move(instructions)};
}

std::uint64_t SolvePart1(const Origami &origami, const std::vector<FoldInstruction> &instructions)
{
	auto folded = PerformFold(origami, instructions.front());
#ifndef NDEBUG
	std::cout << StringifyOrigami(folded, '.');
#endif
	return std::count_if(folded.cells.begin(), folded.cells.end(), [](const Cell &cell)
	{
		return cell == Cell::Dot;
	});
}

std::string SolvePart2(Origami origami, const std::vector<FoldInstruction> &instructions)
{
	for (const auto &instruction: instructions)
	{
		origami = PerformFold(origami, instruction);
	}

	return StringifyOrigami(origami, ' ');
}

Origami PerformFold(const Origami &origami, const FoldInstruction &instruction)
{
	if (instruction.direction == FoldDirection::y)
	{
		const auto topPart = instruction.line;
		const auto bottomPart = origami.height - (instruction.line + 1);

		if (topPart < bottomPart)
		{
			throw std::runtime_error("Size mismatch");
		}
		const auto assumedHeight = origami.height + (topPart - bottomPart);

		std::vector<Cell> newCells;
		newCells.reserve(topPart * origami.width);

		for (auto y = 0U; y < topPart; ++y)
		{
			const auto bottomLine = assumedHeight - 1 - y;
			for (auto x = 0U; x < origami.width; ++x)
			{
				const auto topIndex = y * origami.width + x;
				if (bottomLine < origami.height)
				{
					const auto bottomIndex = bottomLine * origami.width + x;
					newCells.push_back(
							(origami.cells[topIndex] == Cell::Dot || origami.cells[bottomIndex] == Cell::Dot)
							? Cell::Dot
							: Cell::Empty
					);
				}
				else
				{
					newCells.push_back(origami.cells[topIndex]);
				}
			}
		}

		return {
				std::move(newCells),
				origami.width,
				topPart
		};
	}
	else if (instruction.direction == FoldDirection::x)
	{
		const auto leftPart = instruction.line;
		const auto rightPart = origami.width - (instruction.line + 1);

		if (leftPart < rightPart)
		{
			throw std::runtime_error("Size mismatch");
		}

		const auto assumedWidth = origami.width + (leftPart - rightPart);

		std::vector<Cell> newCells;
		newCells.reserve(leftPart * origami.height);


		for (auto y = 0U; y < origami.height; ++y)
		{
			for (auto x = 0U; x < leftPart; ++x)
			{
				const auto rightColumn = assumedWidth - 1 - x;

				const auto leftIndex = y * origami.width + x;
				if (rightColumn < origami.width)
				{
					const auto rightIndex = y * origami.width + rightColumn;
					newCells.push_back(
							(origami.cells[leftIndex] == Cell::Dot || origami.cells[rightIndex] == Cell::Dot)
							? Cell::Dot
							: Cell::Empty
					);
				}
				else
				{
					newCells.push_back(origami.cells[leftIndex]);
				}


			}
		}

		return {
				std::move(newCells),
				leftPart,
				origami.height
		};
	}
	else
	{
		throw std::runtime_error("Invalid instruction");
	}
}

std::string StringifyOrigami(const Origami &origami, char emptyChar)
{
	std::ostringstream stream;
	for (auto y = 0U; y < origami.height; ++y)
	{
		for (auto x = 0U; x < origami.width; ++x)
		{
			const auto index = y * origami.width + x;
			const auto &cell = origami.cells[index];
			if (cell == Cell::Dot)
			{
				stream << "#";
			}
			else
			{
				stream << emptyChar;
			}
		}
		stream << "\r\n";
	}
	return stream.str();
}
