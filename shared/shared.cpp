#include "shared.hpp"

#include <fstream>
#include <sstream>
#include <iterator>

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

	std::transform(lines.begin(), lines.end(), std::back_inserter(result),
	               [](const auto &str)
	               {
		               return StrToInteger<std::uint16_t>(str, 10);
	               });
	return result;
}

std::vector<std::pair<std::string, std::uint16_t>> LinesToStrUint16(const std::vector<std::string> &lines)
{
	std::vector<std::pair<std::string, std::uint16_t>> result;
	for (const auto &line: lines)
	{
		const auto sep = line.find(' ');
		if (sep == std::string::npos)
		{
			throw std::runtime_error("Failed to parse input");
		}
		auto command = line.substr(0, sep);
		const auto argument = StrToInteger<std::uint16_t>({line.begin() + static_cast<long int>(sep) + 1, line.end()},
		                                                  10);
		result.emplace_back(std::move(command), argument);
	}
	return result;
}

std::vector<std::string> SplitStringWhitespace(const std::string &str)
{
	std::istringstream stream{str};
	std::vector<std::string> elements;
	std::copy(std::istream_iterator<std::string>(stream),
	          std::istream_iterator<std::string>(),
	          std::back_inserter(elements));
	return elements;
}

std::vector<std::string> SplitString(const std::string &str, char delimiter)
{
	std::istringstream stream {str};
	std::vector<std::string> elements;
	std::string element;
	while (std::getline(stream, element, delimiter))
	{
		elements.emplace_back(std::move(element));
	}
	return elements;
}

