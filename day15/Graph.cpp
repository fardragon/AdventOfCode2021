
#include "Graph.hpp"
#include <array>
#include <queue>
#include <limits>


Graph::Graph(const std::vector<std::vector<uint8_t>> &nodes)
{
	adjacency.resize(nodes.size() * nodes.front().size());

	for (auto y = 0U; y < nodes.size(); ++y)
	{
		for (auto x = 0U; x < nodes.front().size(); ++ x)
		{
			const auto sourceIndex = y * nodes.front().size() + x;
			const auto neighbours = GetNeighbours({x, y}, nodes.front().size(), nodes.size());

			for (const auto &neighbour: neighbours)
			{
				const auto targetIndex = neighbour.second * nodes.front().size() + neighbour.first;
				const auto weight =nodes[neighbour.second][neighbour.first];
				adjacency[sourceIndex].push_back(std::make_pair(targetIndex, weight));
			}
		}
	}
}

std::vector<Graph::Position>Graph::GetNeighbours(const Graph::Position &currentPosition, std::int64_t maxWidth, std::int64_t maxHeight)
{
	static const std::array<Position, 4> steps =
			{
					std::make_pair(-1, 0),
					std::make_pair(1, 0),
					std::make_pair(0, -1),
					std::make_pair(0, 1),
			};

	std::vector<Position> result {};

	for (const auto &step : steps)
	{
		auto candidate = Position(currentPosition.first + step.first, currentPosition.second + step.second);

		if ((candidate.first >= 0)
			&& (candidate.first < maxWidth)
			&& (candidate.second >= 0)
			&& (candidate.second < maxHeight)
			)
		{
			result.push_back(candidate);
		}


	}

	return result;
}

std::uint64_t Graph::ShortestPath() const
{
	std::priority_queue<std::pair<std::size_t, std::size_t>, std::vector<std::pair<std::size_t, std::size_t>>, std::greater<>> queue;
	std::vector<std::size_t> distances(adjacency.size(), std::numeric_limits<std::size_t>::max());

	queue.push(std::make_pair(0, 0));
	distances[0] = 0;

	while(!queue.empty())
	{
		const auto u = queue.top().second;
		queue.pop();

		for (auto it = adjacency[u].begin(); it != adjacency[u].end(); ++it)
		{
			const auto v = it->first;
			const auto weight = it->second;

			if (distances[v] > distances[u] + weight)
			{
				distances[v] = distances[u] + weight;
				queue.push(std::make_pair(distances[v], v));
			}
		}
	}


	return distances.back();
}
