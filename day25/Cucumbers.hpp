//
// Created by fardragon on 12/25/21.
//

#ifndef ADVENTOFCODE2021_CUCUMBERS_HPP
#define ADVENTOFCODE2021_CUCUMBERS_HPP


#include <vector>
#include <string>

enum class Cucumber
{
	None,
	East,
	South
};



class CucumberField
{
public:
	explicit CucumberField (const std::vector<std::vector<char>> &input);

	bool Step();
	[[nodiscard]] std::string ToString() const;

private:
	static Cucumber CharacterToCucumber(char c);

private:
	std::size_t width;
	std::size_t height;
	std::vector<Cucumber> field;
};


#endif //ADVENTOFCODE2021_CUCUMBERS_HPP
