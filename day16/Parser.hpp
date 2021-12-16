
#ifndef ADVENTOFCODE2021_PARSER_HPP
#define ADVENTOFCODE2021_PARSER_HPP


#include "Packet.hpp"

class Parser
{

public:
	Packet ParsePackets(const std::vector<std::uint8_t> &bits);

private:
	Packet ParsePacket();
	std::uint64_t ParseLiteralValue();
	std::vector<Packet> ParseSubPacketsByLength(std::uint16_t length);
	std::vector<Packet> ParseSubPacketsByNumber(std::uint16_t number);

private:
	std::vector<std::uint8_t>::const_iterator position;
};


#endif //ADVENTOFCODE2021_PARSER_HPP
