
#include "Scanner.hpp"

std::vector<Scanner> Scanner::GetAllRotations() const
{
	auto result = std::vector<Scanner> {};
	result.reserve(24U);

	for (auto i = 0U; i < 24U; ++i)
	{
		result.push_back(Scanner{
				                 name,
				                 {}
		                 }
		);

		result.back().beacons.reserve(beacons.size());
	}

	for(const auto &[x, y, z]: beacons)
	{
		auto ix = 0;

		// x facing up
		result[ix++].beacons.push_back({x, y, z});
		result[ix++].beacons.push_back({x, -z, y});
		result[ix++].beacons.push_back({x, -y, -z});
		result[ix++].beacons.push_back({x, z, -y});

		// x facing down
		result[ix++].beacons.push_back({-x, -y, z});
		result[ix++].beacons.push_back({-x, z, y});
		result[ix++].beacons.push_back({-x, y, -z});
		result[ix++].beacons.push_back({-x, -z, -y});

		// y facing up
		result[ix++].beacons.push_back({y, z, x});
		result[ix++].beacons.push_back({y, -x, z});
		result[ix++].beacons.push_back({y, -z, -x});
		result[ix++].beacons.push_back({y, x, -z});

		// y facing down
		result[ix++].beacons.push_back({-y, -z, x});
		result[ix++].beacons.push_back({-y, x, z});
		result[ix++].beacons.push_back({-y, z, -x});
		result[ix++].beacons.push_back({-y, -x, -z});

		// z facing up
		result[ix++].beacons.push_back({z, x, y});
		result[ix++].beacons.push_back({z, -y, x});
		result[ix++].beacons.push_back({z, -x, -y});
		result[ix++].beacons.push_back({z, y, -x});

		// z facing down
		result[ix++].beacons.push_back({-z, -x, y});
		result[ix++].beacons.push_back({-z, y, x});
		result[ix++].beacons.push_back({-z, x, -y});
		result[ix++].beacons.push_back({-z, -y, -x});
	}
	return result;
}

bool Scanner::Contains(const Beacon &beacon) const
{
	return std::find(beacons.begin(), beacons.end(), beacon) != beacons.end();
}

std::strong_ordering Scanner::operator<=>(const Scanner & other) const
{
	return name <=> other.name;
}

Scanner Scanner::OffsetScanner(const Offset &offset) const
{
	auto result = *this;
	for(auto &beacon: result.beacons)
	{
		beacon.x += std::get<0>(offset);
		beacon.y += std::get<1>(offset);
		beacon.z += std::get<2>(offset);
	}

	return result;
}

Scanner Scanner::operator+(const Scanner &other) const
{
	auto result = *this;
	for (auto &beacon: other.beacons)
	{
		if (!result.Contains(beacon))
		{
			result.beacons.push_back(beacon);
		}
	}
	return result;
}
