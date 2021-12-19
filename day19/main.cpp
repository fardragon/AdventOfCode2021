
#include "shared.hpp"
#include <iostream>
#include <chrono>
#include <set>
#include <vector>
#include <unordered_map>
#include "Beacon.hpp"
#include "Scanner.hpp"


std::vector<Scanner> ParseInput(const std::vector<std::string> &lines);


std::pair<Scanner, std::vector<Offset>> SolveIntermediate(const std::vector<Scanner> &scanners);

std::uint64_t SolvePart1(const Scanner &referenceScanner);

std::uint64_t SolvePart2(const std::vector<Offset> &offsets);


std::optional<Offset> CompareTwoScanners(const Scanner &reference, const Scanner &candidate);


int main(int argc, char **argv)
{
	if (argc < 2)
	{
		throw std::runtime_error("Not enough input arguments");
	}
	const auto begin = std::chrono::steady_clock::now();
	const auto lines = LoadLines(argv[1]);
	[[maybe_unused]] auto input = ParseInput(lines);

	const auto beginSolving = std::chrono::steady_clock::now();
	auto[referenceScanner, offsets] = SolveIntermediate(input);
	const auto part1Result = SolvePart1(referenceScanner);
	const auto part2Result = SolvePart2(offsets);

	const auto end = std::chrono::steady_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	const auto elapsedSolving = std::chrono::duration_cast<std::chrono::microseconds>(end - beginSolving).count();

	std::cout << "Part 1 result: " << part1Result << "\r\n";
	std::cout << "Part 2 result: " << part2Result << "\r\n";
	std::cout << "Time: " << elapsed << "us \r\n";
	std::cout << "Time (without reading and parsing): " << elapsedSolving << "us \r\n";

	return 0;
}

std::vector<Scanner> ParseInput(const std::vector<std::string> &lines)
{
	auto position = lines.cbegin();

	std::vector<Scanner> result;
	while (position != lines.end())
	{
		Scanner scanner{*position, {}};
		std::advance(position, 1); // skip scanner name

		while (position != lines.end() && !position->empty())
		{
			const auto coords = SplitString(*position, ',');
			if (coords.size() != 3)
			{
				throw std::runtime_error("Failed to parse input");
			}
			scanner.beacons.push_back(
					Beacon{
							StrToInteger<std::int32_t>(coords[0]),
							StrToInteger<std::int32_t>(coords[1]),
							StrToInteger<std::int32_t>(coords[2])
					});
			std::advance(position, 1);
		}
		if (position != lines.end())
		{
			std::advance(position, 1); // skip empty line separating scanners
		}
		result.push_back(std::move(scanner));
	}

	return result;
}

std::pair<Scanner, std::vector<Offset>> SolveIntermediate(const std::vector<Scanner> &scanners)
{
	auto referenceScanner = scanners.front();
	std::vector<Offset> offsets{{0, 0, 0}};
	offsets.reserve(scanners.size());

	std::set<std::pair<Scanner, std::vector<Scanner>>> unresolvedScanners;
	for (auto it = std::next(scanners.begin(), 1); it != scanners.end(); ++it)
	{
		unresolvedScanners.insert(std::make_pair(*it, it->GetAllRotations()));
	}

	while (!unresolvedScanners.empty())
	{
		auto foundMatch = false;
		for (const auto &[candidateScanner, rotations]: unresolvedScanners)
		{
			for (const auto &rotation: rotations)
			{
				if (const auto offset = CompareTwoScanners(referenceScanner, rotation); offset.has_value())
				{
					auto rotationOffset = rotation.OffsetScanner(offset.value());
					offsets.push_back(offset.value());
					referenceScanner = referenceScanner + rotationOffset;
					foundMatch = true;
					break;
				}
			}
			if (foundMatch)
			{
				unresolvedScanners.erase(std::make_pair(candidateScanner, rotations));
				break;
			}
		}
		if (!foundMatch)
		{
			throw std::runtime_error("Unable to solve");
		}
	}

	return std::make_pair(referenceScanner, std::move(offsets));
}


std::uint64_t SolvePart1(const Scanner &referenceScanner)
{
	return referenceScanner.beacons.size();
}

std::optional<Offset> CompareTwoScanners(const Scanner &reference, const Scanner &candidate)
{
	for (const auto referenceBeacon: reference.beacons)
	{
		for (const auto candidateBeacon: candidate.beacons)
		{
			auto offsetCandidate = referenceBeacon - candidateBeacon;

			auto matches = std::count_if(candidate.beacons.begin(), candidate.beacons.end(),
			                             [&reference, &offsetCandidate](const Beacon &candidate)
			                             {
				                             return reference.Contains(candidate + offsetCandidate);
			                             });

			if (matches >= 12U)
			{
				return Offset{offsetCandidate.x, offsetCandidate.y, offsetCandidate.z};
			}
		}
	}

	return std::nullopt;
}

std::uint64_t SolvePart2(const std::vector<Offset> &offsets)
{
	std::set<std::uint64_t> distances;

	for (auto source = offsets.begin(); source != offsets.end(); ++source)
	{
		for (auto destination = offsets.begin(); destination != offsets.end(); ++destination)
		{
			if (source != destination)
			{
				const auto manhattan = std::abs(std::get<0>(*destination) - std::get<0>(*source)) +
				                       std::abs(std::get<1>(*destination) - std::get<1>(*source)) +
				                       std::abs(std::get<2>(*destination) - std::get<2>(*source));
				distances.emplace(manhattan);
			}
		}
	}
	return *distances.rbegin();
}