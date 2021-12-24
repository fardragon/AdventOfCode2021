
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <execution>

#include "Instruction.hpp"
#include "ALU.hpp"



std::vector<Instruction> ParseInput(const std::vector<std::string> &lines);

std::pair<std::int64_t, std::int64_t> Solve(const std::vector<Instruction> &instructions);


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
	const auto &[part1Result, part2Result] = Solve(input);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}




std::vector<Instruction> ParseInput(const std::vector<std::string> &lines)
{
	std::vector<Instruction> result{};
	result.reserve(lines.size());

	for (const auto &line: lines)
	{
		result.push_back(ParseInstruction(line));
	}

	return result;
}

std::pair<std::int64_t, std::int64_t> Solve(const std::vector<Instruction> &instructions)
{
	using AluState = std::pair<ALU, std::pair<std::int64_t, std::int64_t>>;

	(void) instructions;
	std::vector<AluState> ALUs { {{}, {0, 0}}};

	for (const auto &instruction: instructions)
	{
		if (instruction.op == Op::Inp)
		{
			std::vector<AluState> newALUs {};
			std::unordered_map<ALU, std::size_t> cache;
			for (const auto &alu: ALUs)
			{
				for (auto digit = 1; digit <= 9; ++digit)
				{
					auto newAlu = alu;
					newAlu.first.ApplyInputInstruction(instruction, digit);
					newAlu.second.first = (newAlu.second.first * 10) + digit;
					newAlu.second.second = (newAlu.second.second * 10) + digit;
					if (cache.contains(newAlu.first))
					{
						const auto index = cache.at(newAlu.first);
						newALUs[index].second.first = std::min(newALUs[index].second.first, newAlu.second.first);
						newALUs[index].second.second = std::max(newALUs[index].second.second, newAlu.second.second);
					}
					else
					{
						cache.emplace(newAlu.first, newALUs.size());
						newALUs.push_back(newAlu);
					}
				}

			}
			ALUs = std::move(newALUs);
			std::cout << "Processing " << ALUs.size() << " ALUs \r\n";
		}
		else
		{
			std::for_each(
					std::execution::par_unseq,
					ALUs.begin(),
					ALUs.end(),
					[&i = std::as_const(instruction)](auto &state)
					{
						state.first.ApplyInstruction(i);
					}
			);
		}
	}

	std::int64_t low = std::numeric_limits<std::int64_t>::max();
	std::int64_t high = std::numeric_limits<std::int64_t>::min();

	for (const auto &[alu, val]: ALUs)
	{
		if (alu.IsSolved())
		{
			low = std::min(low, val.first);
			high = std::max(high, val.second);
		}
	}

	return {high, low};
}
