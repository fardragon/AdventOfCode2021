//
// Created by fardragon on 12/16/21.
//

#include "Parser.hpp"
#include <bitset>
#include <stdexcept>
#include <iostream>

Packet Parser::ParsePackets(const std::vector<std::uint8_t> &bits)
{
	position = bits.begin();

	auto packet = ParsePacket();

	return packet;
}

Packet Parser::ParsePacket()
{
	std::bitset<3> version;
	for (auto i = 0U; i < 3U; ++i)
	{
		version[2 - i] = *position;
		std::advance(position, 1);
	}

	std::bitset<3> type;
	for (auto i = 0U; i < 3U; ++i)
	{
		type[2 - i] = *position;
		std::advance(position, 1);
	}

	auto versionInt = version.to_ulong();
	auto typeInt = type.to_ulong();

	if (typeInt == 4)
	{
		// literal packet
		const auto value = ParseLiteralValue();

		return {
			static_cast<uint8_t>(versionInt),
			static_cast<uint8_t>(typeInt),
			value
		};
	}
	else
	{
		// operator packet
		if (*position == 0)
		{
			// sub-packets length
			std::advance(position, 1);
			std::bitset<15> length;
			for (auto i = 0U; i < length.size(); ++i)
			{
				length[length.size() - 1 - i] = *position;
				std::advance(position, 1);
			}
			auto lengthInt = static_cast<std::uint16_t>(length.to_ullong());
			return {
					static_cast<uint8_t>(versionInt),
					static_cast<uint8_t>(typeInt),
					ParseSubPacketsByLength(lengthInt)
			};
		}
		else
		{
			// sub-packets number
			std::advance(position, 1);
			std::bitset<11> length;
			for (auto i = 0U; i < length.size(); ++i)
			{
				length[length.size() - 1 - i] = *position;
				std::advance(position, 1);
			}
			auto lengthInt = static_cast<std::uint16_t>(length.to_ullong());
			return {
					static_cast<uint8_t>(versionInt),
					static_cast<uint8_t>(typeInt),
					ParseSubPacketsByNumber(lengthInt)
			};
		}
	}
}

std::uint64_t Parser::ParseLiteralValue()
{
	auto stop = false;

	std::uint64_t result {};
	while(!stop)
	{
		if (*position == 0)
		{
			stop = true;
		}
		std::advance(position, 1);

		std::bitset<4> value;
		for (auto i = 0U; i < 4U; ++i)
		{
			value[3 - i] = *position;
			std::advance(position, 1);
		}

		result <<= 4;
		result |= value.to_ulong();
	}
	return result;
}

std::vector<Packet> Parser::ParseSubPacketsByLength(std::uint16_t length)
{
	std::vector<Packet> subPackets {};
	auto startPos = position;

	while (std::distance(startPos, position) < length)
	{
		subPackets.push_back(ParsePacket());
	}

	return subPackets;
}

std::vector<Packet> Parser::ParseSubPacketsByNumber(std::uint16_t number)
{
	std::vector<Packet> subPackets {};
	subPackets.reserve(number);
	for (auto i=0U; i < number; ++i)
	{
		subPackets.push_back(ParsePacket());
	}
	return subPackets;
}
