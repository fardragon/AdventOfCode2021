#ifndef ADVENTOFCODE2021_SHARED_HPP
#define ADVENTOFCODE2021_SHARED_HPP

#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <charconv>

[[nodiscard]] std::vector<std::string> LoadLines(const std::string &path);

template <class N>
[[nodiscard]] N StrToInteger(std::string_view sv, int base = 10);
[[nodiscard]] std::vector<std::uint16_t> LinesToUint16(const std::vector<std::string> &lines);
[[nodiscard]] std::vector<std::pair<std::string, std::uint16_t>> LinesToStrUint16(const std::vector<std::string> &lines);

template<class N>
N StrToInteger(std::string_view sv, int base)
{
	static_assert(std::is_integral_v<N>);
	N value;
	const auto res = std::from_chars(sv.begin(), sv.end(), value, base);
	if (res.ec != std::errc())
	{
		throw std::runtime_error("Failed to parse input");
	}
	return value;
}

#endif //ADVENTOFCODE2021_SHARED_HPP
