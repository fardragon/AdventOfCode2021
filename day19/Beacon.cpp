//
// Created by fardragon on 12/19/21.
//

#include "Beacon.hpp"

Beacon Beacon::operator-(const Beacon &other) const
{
	return {x - other.x, y - other.y, z - other.z};
}

Beacon Beacon::operator+(const Beacon &other) const
{
	return {x + other.x, y + other.y, z + other.z};
}
