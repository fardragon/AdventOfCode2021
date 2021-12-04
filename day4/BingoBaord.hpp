#ifndef ADVENTOFCODE2021_BINGOBAORD_HPP
#define ADVENTOFCODE2021_BINGOBAORD_HPP

#include <vector>
#include <cstdint>
#include <array>
#include <optional>

class Board
{
public:
	Board(std::uint8_t boardNumber, const std::vector<std::uint8_t>& elements);
	std::optional<std::uint32_t> MarkNumber(std::uint8_t number);
	[[nodiscard]] std::uint8_t GetBoardNumber() const;

private:
	std::optional<std::uint32_t> CheckSolved(std::uint8_t markedNumber);
	std::uint32_t CalculateResult(std::uint8_t markedNumber);
private:
	std::uint8_t _boardNumber;
	std::array<std::pair<std::uint8_t, bool>, 25> _elements;
};


#endif //ADVENTOFCODE2021_BINGOBAORD_HPP
