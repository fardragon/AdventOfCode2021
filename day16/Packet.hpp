#ifndef ADVENTOFCODE2021_PACKET_HPP
#define ADVENTOFCODE2021_PACKET_HPP

#include <vector>
#include <cstdint>
#include <variant>

struct Packet
{
	std::uint8_t version;
	std::uint8_t type;
	std::variant<std::uint64_t, std::vector<Packet>> content;

	[[nodiscard]] std::uint64_t GetVersionSum() const noexcept;
	[[nodiscard]] std::uint64_t Solve() const;
};



#endif //ADVENTOFCODE2021_PACKET_HPP
