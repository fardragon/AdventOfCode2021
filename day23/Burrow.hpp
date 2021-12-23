#ifndef ADVENTOFCODE2021_BURROW_HPP
#define ADVENTOFCODE2021_BURROW_HPP


#include <cstdint>
#include <array>
#include <map>
#include <optional>
#include <utility>
#include <stdexcept>
#include <iostream>

enum class Field
{
	A,
	B,
	C,
	D,
	Empty
};

template<std::size_t N>
class Burrow
{
public:
	explicit Burrow(std::array<std::array<char, N>, 4> initialState);

	[[nodiscard]] std::uint64_t Solve() const;
	std::strong_ordering operator<=>(const Burrow &other) const = default;

private:
	using BurrowCache = std::map<Burrow, std::uint64_t>;
	[[nodiscard]] std::uint64_t SolveInternal(BurrowCache &cache) const;

	[[nodiscard]] static std::array<Field, N> ParseFields(const std::array<char, N> &input);
	[[nodiscard]] bool IsSolved() const;
	[[nodiscard]] std::optional<std::uint8_t> MoveFromHallway(uint8_t index) const;
	[[nodiscard]] bool CanMoveInHallway(uint8_t source, uint8_t destination) const;
	[[nodiscard]] std::vector<std::tuple<Field, std::uint8_t, std::uint8_t>> MoveFromRoom(const Field &room) const;
	void AddToRoom(const Field &f);
	void RemoveFromRoom(const Field &f);
	[[nodiscard]] bool RoomSolved(const Field &f) const;
	[[nodiscard]] bool RoomContainsOnlyOwn(const Field &f) const;
	[[nodiscard]] bool RoomEmpty(const Field &f) const;
	[[nodiscard]] std::pair<Field, std::size_t> RoomTop(const Field &room) const;
	[[nodiscard]] std::pair<bool, std::uint8_t> CanMoveIntoRoom(const Field &f) const;

private:
	std::map<Field, std::array<Field, N>> rooms;
	std::map<Field, std::uint8_t> roomsPositions;
	std::map<Field, std::uint16_t> moveCosts;
	std::array<Field, 11> hallway = {Field::Empty, Field::Empty, Field::Empty, Field::Empty, Field::Empty, Field::Empty,
	                                 Field::Empty, Field::Empty, Field::Empty, Field::Empty, Field::Empty};
};

template<std::size_t N>
Burrow<N>::Burrow(std::array<std::array<char, N>, 4> initialState)
{
	rooms.emplace(Field::A, std::move(ParseFields(initialState[0])));
	rooms.emplace(Field::B, std::move(ParseFields(initialState[1])));
	rooms.emplace(Field::C, std::move(ParseFields(initialState[2])));
	rooms.emplace(Field::D, std::move(ParseFields(initialState[3])));

	roomsPositions.emplace(Field::A, 2);
	roomsPositions.emplace(Field::B, 4);
	roomsPositions.emplace(Field::C, 6);
	roomsPositions.emplace(Field::D, 8);

	moveCosts.emplace(Field::A, 1);
	moveCosts.emplace(Field::B, 10);
	moveCosts.emplace(Field::C, 100);
	moveCosts.emplace(Field::D, 1000);
}

template<std::size_t N>
std::uint64_t Burrow<N>::Solve() const
{
	BurrowCache cache;
	return SolveInternal(cache);
}

template<std::size_t N>
bool Burrow<N>::IsSolved() const
{
	auto solved = true;
	for (const auto &room: rooms)
	{
		solved &= RoomSolved(room.first);
	}
	return solved;
}

template<std::size_t N>
std::optional<std::uint8_t> Burrow<N>::MoveFromHallway(uint8_t index) const
{
	const auto field = hallway.at(index);
	const auto roomIndex = roomsPositions.at(field);

	if (!CanMoveInHallway(index, roomIndex))
	{
		return std::nullopt;
	}

	const std::uint8_t hallwayDistance = std::abs(static_cast<std::int8_t>(index) - static_cast<std::int8_t>(roomIndex));
	const auto &[canMove, depth] = CanMoveIntoRoom(field);

	if (canMove)
	{
		return hallwayDistance + depth;
	}
	else
	{
		return std::nullopt;
	}
}

template<std::size_t N>
void Burrow<N>::AddToRoom(const Field &f)
{
	for(auto it = rooms.at(f).rbegin(); it != rooms.at(f).rend(); ++it)
	{
		if (*it == Field::Empty)
		{
			*it = f;
			break;
		}
	}
}

template<std::size_t N>
bool Burrow<N>::RoomSolved(const Field &f) const
{
	return std::all_of(rooms.at(f).begin(), rooms.at(f).end(),
	                      [f](const Field &t)
	                      {
		                      return t == f;
	                      });
}

template<std::size_t N>
void Burrow<N>::RemoveFromRoom(const Field &f)
{
	for(auto it = rooms.at(f).begin(); it != rooms.at(f).end(); ++it)
	{
		if (*it != Field::Empty)
		{
			*it = Field::Empty;
			return;
		}
	}
	throw std::runtime_error("Unreachable");
}

template<std::size_t N>
std::vector<std::tuple<Field, std::uint8_t, std::uint8_t>> Burrow<N>::MoveFromRoom(const Field &room) const
{
	if (RoomEmpty(room) || RoomContainsOnlyOwn(room) || RoomSolved(room))
	{
		return {};
	}

	const auto startingPos = roomsPositions.at(room);
	const auto &[field, depth] = RoomTop(room);

	std::vector<std::tuple<Field, std::uint8_t, std::uint8_t>> result {};

	for (auto i = 0UL; i < hallway.size(); ++i)
	{
		auto skip = false;
		for (const auto &[_, roomPos]: roomsPositions)
		{
			if (i == roomPos)
			{
				skip = true;
				break;
			}
		}

		if (skip)
		{
			continue;
		}

		if (CanMoveInHallway(startingPos, i))
		{
			result.emplace_back(field, i, (i > startingPos ? i - startingPos : startingPos - i) + depth);
		}
	}

	return result;
}

template<std::size_t N>
std::array<Field, N> Burrow<N>::ParseFields(const std::array<char, N> &input)
{
	std::array<Field, N> result {};
	for (auto i = 0U; i < N; ++i)
	{
		switch (input[i])
		{
			case 'A':
				result[i] = Field::A;
				break;
			case 'B':
				result[i] = Field::B;
				break;
			case 'C':
				result[i] = Field::C;
				break;
			case 'D':
				result[i] = Field::D;
				break;
			default:
				throw std::runtime_error("Failed to parse fields");
		}
	}
	return result;
}

template<std::size_t N>
std::pair<bool, std::uint8_t> Burrow<N>::CanMoveIntoRoom(const Field &f) const
{
	std::uint8_t depth = 0;
	for(const auto &field: rooms.at(f))
	{
		if (field == Field::Empty)
		{
			depth++;
		}
		else if (field != f)
		{
			return {false, depth};
		}
	}
	return {depth > 0, depth};
}

template<std::size_t N>
bool Burrow<N>::RoomEmpty(const Field &f) const
{
	return std::all_of(rooms.at(f).begin(), rooms.at(f).end(),
	                   [](const Field &t)
	                   {
		                   return t == Field::Empty;
	                   });
}

template<std::size_t N>
bool Burrow<N>::CanMoveInHallway(std::uint8_t source, std::uint8_t destination) const
{
	for(auto i = std::min(source, destination); i <= std::max(source, destination); ++i)
	{
		if (i == source)
		{
			continue;
		}
		else if (hallway.at(i) != Field::Empty)
		{
			return false;
		}
	}
	return true;
}

template<std::size_t N>
std::pair<Field, std::size_t> Burrow<N>::RoomTop(const Field &room) const
{
	std::size_t depth = 1;
	for(auto it = rooms.at(room).begin(); it != rooms.at(room).end(); ++it)
	{
		if (*it != Field::Empty)
		{
			return {*it, depth};
		}
		++depth;
	}
	throw std::runtime_error("Unreachable");
}

template<std::size_t N>
bool Burrow<N>::RoomContainsOnlyOwn(const Field &f) const
{
	return std::all_of(rooms.at(f).begin(), rooms.at(f).end(),
	                   [f](const Field &t)
	                   {
		                   return (t == Field::Empty) || (t == f);
	                   });
}

template<std::size_t N>
std::uint64_t Burrow<N>::SolveInternal(Burrow::BurrowCache &cache) const
{
	if (IsSolved())
	{
		return 0;
	}

	if(cache.contains(*this))
	{
		return cache.at(*this);
	}

	// Try to move from hallway
	for (auto i = 0U; i < hallway.size(); ++i)
	{
		if (hallway.at(i) == Field::Empty)
		{
			continue;
		}

		auto distance = MoveFromHallway(i);
		if (!distance.has_value())
		{
			continue;
		}

		const auto cost = static_cast<std::uint64_t>(distance.value()) * moveCosts.at(hallway.at(i));
		auto newState = *this;

		newState.AddToRoom(hallway.at(i));
		newState.hallway.at(i) =  Field::Empty;
//		std::cout << "Moving from hallway " << i << " to room\r\n";
		return cost + newState.SolveInternal(cache);
	}

	// Try to move into hallway
	std::uint64_t result = std::numeric_limits<std::uint64_t>::max() / 2;
	for (auto &room: rooms)
	{
		auto moves = MoveFromRoom(room.first);
		if (moves.empty())
		{
			continue;
		}

		for (const auto &[field, index, distance]: moves)
		{
			const auto cost = distance * moveCosts.at(field);

			auto newState = *this;
			newState.hallway.at(index) = field;
			newState.RemoveFromRoom(room.first);
//			std::cout << "Moving from room " << static_cast<int>(room.first) << " to hallway " << static_cast<int>(index) << "\r\n";

			auto solveCost = newState.SolveInternal(cache) + cost;
			if (solveCost < result)
			{
				result = solveCost;
			}
		}
	}

	cache.emplace(*this, result);
	return result;
}

#endif //ADVENTOFCODE2021_BURROW_HPP
