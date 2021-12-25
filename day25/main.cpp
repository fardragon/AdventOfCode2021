
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include "Cucumbers.hpp"


std::vector<std::vector<char>> ParseInput(const std::vector<std::string> &lines);
std::uint64_t Solve(const std::vector<std::vector<char>> &input);

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
	const auto result = Solve(input);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Result: " << result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::vector<std::vector<char>> ParseInput(const std::vector<std::string> &lines)
{
	std::vector<std::vector<char>> result;
	result.reserve(lines.size());

	const auto width = lines.front().size();
	for (const auto &line: lines)
	{
		if (line.size() != width)
		{
			throw std::runtime_error("Failed to parse input");
		}

		std::vector<char> row;
		row.reserve(line.size());
		for (const auto &c: line)
		{
			if (c == '>' || c == 'v' || c == '.')
			{
				row.push_back(c);
			}
			else
			{
				throw std::runtime_error("Failed to parse input");
			}
		}
		result.push_back(std::move(row));
	}
	return result;
}
std::uint64_t Solve(const std::vector<std::vector<char>> &input)
{
	CucumberField field(input);
#ifndef NDEBUG
	std::cout << field.ToString() << "__________\r\n";
#endif

	std::uint64_t counter {};
	do
	{
		++counter;
	}
	while(field.Step());

#ifndef NDEBUG
	std::cout << field.ToString() << "__________\r\n";
#endif
	return counter;
}
