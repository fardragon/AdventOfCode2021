#include "BingoBaord.hpp"
#include <stdexcept>
#include <numeric>

Board::Board(std::uint8_t boardNumber, const std::vector<std::uint8_t> &elements) :
		_boardNumber(boardNumber), _elements()
{
	if (elements.size() != _elements.size())
	{
		throw std::runtime_error("Invalid board elements size");
	}

	for (auto i = 0U; i < elements.size(); ++i)
	{
		_elements[i] = {elements[i], false};
	}
}

std::optional<std::uint32_t> Board::MarkNumber(std::uint8_t number)
{
	for (auto &element: _elements)
	{
		if (element.first == number)
		{
			element.second = true;
			break;
		}
	}
	return CheckSolved(number);
}

std::optional<std::uint32_t> Board::CheckSolved(std::uint8_t markedNumber)
{
	// check rows
	for (auto i = 0U; i < _elements.size(); i += 5)
	{
		const auto begin = std::next(_elements.begin(), i);
		const auto end = std::next(begin, 5);
		const auto solved = std::all_of(begin, end,
		                                [](const auto &elem)
		                                {
			                                return elem.second;
		                                }
		);

		if (solved)
		{
			return CalculateResult(markedNumber);
		}
	}

	// check columns
	for (auto i = 0U; i < 5U; ++i)
	{
		auto columnSolved = true;
		for (auto c = i; c < _elements.size(); c += 5)
		{
			columnSolved &= _elements[c].second;
		}

		if (columnSolved)
		{
			return CalculateResult(markedNumber);
		}
	}
	return std::nullopt;
}

std::uint32_t Board::CalculateResult(std::uint8_t markedNumber)
{
	std::uint32_t result {};
	for(const auto &element: _elements)
	{
		if (!element.second)
		{
			result += element.first;
		}
	}
	return result * markedNumber;
}

std::uint8_t Board::GetBoardNumber() const
{
	return _boardNumber;
}
