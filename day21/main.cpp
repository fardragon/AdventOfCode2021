
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <set>


struct Player
{
	std::uint8_t position;
	std::uint16_t score;
	std::strong_ordering operator<=>(const Player &) const = default;
};



std::pair<Player, Player> ParseInput(const std::vector<std::string> &lines);

std::uint64_t SolvePart1(std::pair<Player, Player> players);
std::uint64_t SolvePart2(std::pair<Player, Player> players);

using QuantumCache = std::map<std::pair<Player, Player>, std::pair<std::uint64_t, std::uint64_t>>;
std::pair<std::uint64_t, std::uint64_t> QuantumGame(std::pair<Player, Player> players, QuantumCache &cache);

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

std::pair<Player, Player> ParseInput(const std::vector<std::string> &lines)
{
	if (lines.size() != 2)
	{
		throw std::runtime_error("Failed to parse input");
	}

	auto p1Elems = SplitStringWhitespace(lines.front());
	auto p2Elems = SplitStringWhitespace(lines.at(1));


	// subtract one from positions to make them 0 based
	Player p1{
			static_cast<uint8_t>(StrToInteger<std::uint8_t>(p1Elems.back()) - 1),
			0
	};

	Player p2{
			static_cast<uint8_t>(StrToInteger<std::uint8_t>(p2Elems.back()) - 1),
			0
	};

	return {p1, p2};
};

std::uint64_t SolvePart1(std::pair<Player, Player> players)
{
	std::uint16_t rolls{};

	const auto die = []()
	{
		static std::uint8_t die = 1;
		if (die == 100)
		{
			die = 1;
			return static_cast<std::uint8_t>(100U);
		}
		else
		{
			return die++;
		}
	};

	auto p1Turn = true;
	while (players.first.score < 1000 && players.second.score < 1000)
	{
		auto &currentPlayer = p1Turn ? players.first : players.second;
		auto move = die() + die() + die();
		rolls += 3;

		currentPlayer.position = (currentPlayer.position + move) % 10;
		currentPlayer.score += (currentPlayer.position + 1);
		p1Turn = !p1Turn;
	}

	const auto &losingPlayer = p1Turn ? players.first : players.second;
	return losingPlayer.score * rolls;
}

std::uint64_t SolvePart2(std::pair<Player, Player> players)
{
	QuantumCache quantumCache;
	const auto &[p1, p2] = QuantumGame(players, quantumCache);
	return std::max(p1, p2);
}

std::pair<std::uint64_t, std::uint64_t> QuantumGame(std::pair<Player, Player> players, QuantumCache &cache)
{
	if (players.first.score >= 21)
	{
		return {1, 0};
	}
	else if (players.second.score >= 21)
	{
		return {0, 1};
	}
	else if (cache.contains(players))
	{
		return cache.at(players);
	}

	std::pair<std::uint64_t, std::uint64_t> result {0, 0};

	for (auto die1 = 1U; die1 <= 3U; ++die1)
	{
		for (auto die2 = 1U; die2 <= 3U; ++die2)
		{
			for (auto die3 = 1U; die3 <= 3U; ++die3)
			{
				auto currentPlayer = players.first;
				currentPlayer.position = (currentPlayer.position + die1 + die2 + die3) % 10;
				currentPlayer.score += (currentPlayer.position + 1);

				auto newPlayers = std::make_pair(players.second, currentPlayer);

				const auto &[p2Wins, p1Wins] = QuantumGame(newPlayers, cache);
				result.first += p1Wins;
				result.second += p2Wins;
			}
		}
	}

	cache[players] = result;
	return result;
}
