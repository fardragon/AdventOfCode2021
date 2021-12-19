#ifndef ADVENTOFCODE2021_SCANNER_HPP
#define ADVENTOFCODE2021_SCANNER_HPP

#include "Beacon.hpp"
#include <vector>
#include <string>

class Scanner
{
public:
	std::string name;
	std::vector<Beacon> beacons;

	std::strong_ordering operator<=>(const Scanner &other) const;
	Scanner operator+(const Scanner &other) const;

public:
	[[nodiscard]] std::vector<Scanner> GetAllRotations() const;
	[[nodiscard]] bool Contains(const Beacon &beacon) const;
	[[nodiscard]] Scanner OffsetScanner(const Offset &offset) const;
};


#endif //ADVENTOFCODE2021_SCANNER_HPP
