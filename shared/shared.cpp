#include "shared.hpp"

#include <fstream>
#include <stdexcept>
#include <charconv>

std::vector<std::string> LoadLines(const std::string &path)
{
	std::ifstream file(path);

	if (!file.good())
	{
		throw std::runtime_error("Failed to open file");
	}

	std::vector<std::string> result;
	std::string line{};

	while (std::getline(file, line))
	{
		result.push_back(line);
	}

	return result;
}

std::vector<std::uint16_t> LinesToUint16(const std::vector<std::string> &lines)
{
	std::vector<std::uint16_t> result;
	result.reserve(lines.size());

	for (const auto &line: lines)
	{
		uint16_t value;
		const auto res = std::from_chars(line.data(), line.data() + line.length(), value);
		if (res.ec != std::errc())
		{
			throw std::runtime_error("Failed to parse input");
		}
		result.push_back(value);
	}
	return result;
}
