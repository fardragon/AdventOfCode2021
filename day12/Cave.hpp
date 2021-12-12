#ifndef ADVENTOFCODE2021_CAVE_HPP
#define ADVENTOFCODE2021_CAVE_HPP

#include <string>
#include <cctype>
#include <vector>
#include <unordered_map>



struct Cave
{
	std::string name;
	std::vector<std::string> connections;
	[[nodiscard]] bool IsBig() const;
};


#endif //ADVENTOFCODE2021_CAVE_HPP
