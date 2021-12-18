
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <set>
#include <vector>

#include "Node.hpp"



std::vector<std::unique_ptr<Node>> ParseInput(const std::vector<std::string> &lines);
std::uint64_t SolvePart1(const std::vector<std::unique_ptr<Node>> &nodes);
std::uint64_t SolvePart2(const std::vector<std::unique_ptr<Node>> &nodes);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	auto input = ParseInput(lines);

	const auto beginSolving = std::chrono::steady_clock::now();
	const auto part1Result = SolvePart1(input);
	const auto part2Result = SolvePart2(input);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::vector<std::unique_ptr<Node>> ParseInput(const std::vector<std::string> &lines)
{
	std::vector<std::unique_ptr<Node>> result {};
	result.reserve(lines.size());

	NodeParser parser {};
	for (const auto &line: lines)
	{
		result.push_back(parser.Parse(line));
	}

	return result;
}

std::uint64_t SolvePart1(const std::vector<std::unique_ptr<Node>> &nodes)
{
	auto node = nodes.front()->Copy();


	for (auto it = std::next(nodes.cbegin(), 1); it != nodes.cend(); std::advance(it, 1))
	{
		auto newNode = std::make_unique<Node>(NodeOrientation::Top, std::make_pair(nullptr, nullptr));
		node->SetParent(newNode.get());
		node->SetOrientation(NodeOrientation::Left);

		auto addNode = (*it)->Copy();
		addNode->SetParent(newNode.get());
		addNode->SetOrientation(NodeOrientation::Right);

		newNode->SetNested(std::make_pair(std::move(node), std::move(addNode)));

		node = std::move(newNode);
		node->Reduce();
	}

	return node->GetMagnitude();
}

std::uint64_t SolvePart2(const std::vector<std::unique_ptr<Node>> &nodes)
{
	std::set<std::uint64_t> results;

	for (auto it1 = nodes.cbegin(); it1 != nodes.cend(); std::advance(it1, 1))
	{
		for (auto it2 = nodes.cbegin(); it2 != nodes.cend(); std::advance(it2, 1))
		{
			if (it1 != it2)
			{
				auto newNode = std::make_unique<Node>(NodeOrientation::Top, std::make_pair(nullptr, nullptr));

				auto lNode = (*it1)->Copy();
				lNode->SetParent(newNode.get());
				lNode->SetOrientation(NodeOrientation::Left);

				auto rNode = (*it2)->Copy();
				rNode->SetParent(newNode.get());
				rNode->SetOrientation(NodeOrientation::Right);

				newNode->SetNested(std::make_pair(std::move(lNode), std::move(rNode)));

				newNode->Reduce();
				results.emplace(newNode->GetMagnitude());
			}
		}
	}

	return *results.rbegin();
}