#ifndef ADVENTOFCODE2021_SHARED_HPP
#define ADVENTOFCODE2021_SHARED_HPP

#include <vector>
#include <string>
#include <utility>

[[nodiscard]] std::vector<std::string> LoadLines(const std::string &path);
[[nodiscard]] std::uint16_t StrToUint16(std::string_view sv);
[[nodiscard]] std::vector<std::uint16_t> LinesToUint16(const std::vector<std::string> &lines);
[[nodiscard]] std::vector<std::pair<std::string, std::uint16_t>> LinesToStrUint16(const std::vector<std::string> &lines);

#endif //ADVENTOFCODE2021_SHARED_HPP
