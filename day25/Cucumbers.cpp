//
// Created by fardragon on 12/25/21.
//

#include "Cucumbers.hpp"
#include <stdexcept>
#include <sstream>

Cucumber CucumberField::CharacterToCucumber(char c)
{
	switch (c)
	{
		case 'v':
			return Cucumber::South;
		case '>':
			return Cucumber::East;
		case '.':
			return Cucumber::None;
		default:
			throw std::runtime_error("Unknown cucumber");
	}
}

CucumberField::CucumberField(const std::vector<std::vector<char>> &input)
{
	height = input.size();
	width = input.front().size();
	field.reserve(width * height);

	for (const auto &row: input)
	{
		for (const auto &c: row)
		{
			field.push_back(CharacterToCucumber(c));
		}
	}
}


bool CucumberField::Step()
{
	std::vector<Cucumber>  horizontalField(width * height, Cucumber::None);
	auto frozen = true;

	for (auto y = 0U; y < height; ++y)
	{
		for (auto x = 0U; x < width; ++x)
		{
			const auto index = y * width + x;
			switch (field[index])
			{
				case Cucumber::None:
					continue;
				case Cucumber::East:
				{
					const auto targetX = (x + 1) % width;
					const auto targetIndex = y * width + targetX;

					switch (field[targetIndex])
					{
						case Cucumber::None:
						{
							frozen = false;
							horizontalField[index] = Cucumber::None;
							horizontalField[targetIndex] = Cucumber::East;
							break;
						}
						case Cucumber::East: [[fallthrough]];
						case Cucumber::South:
						{
							horizontalField[index] = Cucumber::East;
							break;
						}
					}
					break;
				}
				case Cucumber::South:
					horizontalField[index] = Cucumber::South;
					break;
			}
		}
	}

	std::vector<Cucumber>  verticalField(horizontalField);
	for (auto y = 0U; y < height; ++y)
	{
		for (auto x = 0U; x < width; ++x)
		{
			const auto index = y * width + x;
			switch (horizontalField[index])
			{
				case Cucumber::None:
					continue;
				case Cucumber::South:
				{
					const auto targetY = (y + 1) % height;
					const auto targetIndex = targetY * width + x;

					switch (horizontalField[targetIndex])
					{
						case Cucumber::None:
						{
							frozen = false;
							verticalField[index] = Cucumber::None;
							verticalField[targetIndex] = Cucumber::South;
							break;
						}
						case Cucumber::East: [[fallthrough]];
						case Cucumber::South:
						{
							verticalField[index] = Cucumber::South;
							break;
						}
					}
					break;
				}
				case Cucumber::East:
					verticalField[index] = Cucumber::East;
					break;
			}
		}
	}
	field = std::move(verticalField);
	return !frozen;
}

std::string CucumberField::ToString() const
{
	std::ostringstream oss;
	for (auto y = 0U; y < height; ++y)
	{
		for (auto x = 0U; x < width; ++x)
		{
			const auto index = y * width + x;
			switch (field[index])
			{
				case Cucumber::None:
					oss << '.';
					break;
				case Cucumber::East:
					oss << '>';
					break;
				case Cucumber::South:
					oss << 'v';
					break;
			}
		}
		oss << "\r\n";
	}

	return oss.str();
}

