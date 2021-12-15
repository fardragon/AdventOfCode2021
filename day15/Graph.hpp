#ifndef ADVENTOFCODE2021_GRAPH_HPP
#define ADVENTOFCODE2021_GRAPH_HPP

#include <vector>
#include <cstdint>

class Graph
{
public:
	using Position = std::pair<std::int64_t , std::int64_t>;

	explicit Graph(const std::vector<std::vector<uint8_t>> &nodes);
	std::uint64_t ShortestPath() const;


private:
	std::vector<Position> GetNeighbours(const Position &currentPosition, int64_t maxWidth, int64_t maxHeight);

private:
	std::vector<std::vector<std::pair<std::size_t, std::size_t>>> adjacency;
};


#endif //ADVENTOFCODE2021_GRAPH_HPP
