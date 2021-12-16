#include "Packet.hpp"

#include <algorithm>

std::uint64_t Packet::GetVersionSum() const noexcept
{
	std::uint64_t versionSum{version};

	if (type != 4)
	{

		for (const auto &sub: std::get<std::vector<Packet>>(content))
		{
			versionSum += sub.GetVersionSum();
		}
	}

	return versionSum;
}

std::uint64_t Packet::Solve() const
{
	switch (type)
	{
		case 0:
		{
			std::uint64_t sum{};
			for (const auto &sub: std::get<std::vector<Packet>>(content))
			{
				sum += sub.Solve();
			}
			return sum;
		}
		case 1:
		{
			std::uint64_t product{1};
			for (const auto &sub: std::get<std::vector<Packet>>(content))
			{
				product *= sub.Solve();
			}
			return product;
		}
		case 2:
		{
			const auto &subPackets = std::get<std::vector<Packet>>(content);
			return std::min_element(subPackets.begin(), subPackets.end(),
			                        [](const Packet &a, const Packet &b)
			                        {
				                        return a.Solve() < b.Solve();
			                        }
			)->Solve();
		}
		case 3:
		{
			const auto &subPackets = std::get<std::vector<Packet>>(content);
			return std::min_element(subPackets.begin(), subPackets.end(),
			                        [](const Packet &a, const Packet &b)
			                        {
				                        return a.Solve() > b.Solve();
			                        }
			)->Solve();
		}
		case 4:
		{
			return std::get<std::uint64_t>(content);
		}
		case 5:
		{
			const auto &subPackets = std::get<std::vector<Packet>>(content);
			return subPackets[0].Solve() > subPackets[1].Solve() ? 1U: 0U;
		}
		case 6:
		{
			const auto &subPackets = std::get<std::vector<Packet>>(content);
			return subPackets[0].Solve() < subPackets[1].Solve() ? 1U: 0U;
		}
		case 7:
		{
			const auto &subPackets = std::get<std::vector<Packet>>(content);
			return subPackets[0].Solve() == subPackets[1].Solve() ? 1U: 0U;
		}
		default:
			throw std::runtime_error("Unknown type");
	}
}
