
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <stack>

std::uint64_t SolvePart1(const std::vector<std::string> &lines);
std::uint64_t SolvePart2(const std::vector<std::string> &lines);

std::uint64_t ValidateLine(const std::string &line);
std::uint64_t RepairLine(const std::string &line);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);

	const auto beginSolving = std::chrono::steady_clock::now();
	const auto part1Result = SolvePart1(lines);
	const auto part2Result = SolvePart2(lines);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::uint64_t SolvePart1(const std::vector<std::string> &lines)
{
	std::uint64_t result {};

	for (const auto &line: lines)
	{
		result += ValidateLine(line);
	}

	return result;
}

std::uint64_t SolvePart2(const std::vector<std::string> &lines)
{
	std::vector<std::uint64_t> scores;
	scores.reserve(lines.size());

	for (const auto &line: lines)
	{
		if (const auto score = RepairLine(line); score > 0)
		{
			scores.push_back(score);
		}
	}

	std::sort(scores.begin(), scores.end());

	return scores[scores.size() / 2];
}

std::uint64_t ValidateLine(const std::string &line)
{
	std::stack<char> stack;

	for (const auto &c: line)
	{
		switch (c)
		{
			case '(':
			case '[':
			case '{':
			case '<':
				stack.push(c);
				break;
			case ')':
			{
				if (stack.top() != '(')
				{
					return 3;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case ']':
			{
				if (stack.top() != '[')
				{
					return 57;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case '}':
			{
				if (stack.top() != '{')
				{
					return 1197;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case '>':
			{
				if (stack.top() != '<')
				{
					return 25137;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			default:
				throw std::runtime_error("Unexpected character");
		}
	}


	return 0;
}

std::uint64_t RepairLine(const std::string &line)
{
	std::stack<char> stack;

	for (const auto &c: line)
	{
		switch (c)
		{
			case '(':
			case '[':
			case '{':
			case '<':
				stack.push(c);
				break;
			case ')':
			{
				if (stack.top() != '(')
				{
					return 0;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case ']':
			{
				if (stack.top() != '[')
				{
					return 0;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case '}':
			{
				if (stack.top() != '{')
				{
					return 0;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			case '>':
			{
				if (stack.top() != '<')
				{
					return 0;
				}
				else
				{
					stack.pop();
					break;
				}
			}
			default:
				throw std::runtime_error("Unexpected character");
		}
	}

	std::uint64_t result {};
	while (!stack.empty())
	{
		const auto c = stack.top();

		result *= 5;
		switch (c)
		{
			case '(':
				result += 1;
				break;
			case '[':
				result += 2;
				break;
			case '{':
				result += 3;
				break;
			case '<':
				result += 4;
				break;
			default:
				throw std::runtime_error("Unexpected character");
		}

		stack.pop();
	}

	return result;
}
