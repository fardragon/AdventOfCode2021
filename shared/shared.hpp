#ifndef ADVENTOFCODE2021_SHARED_HPP
#define ADVENTOFCODE2021_SHARED_HPP

#include <vector>
#include <string>

[[nodiscard]] std::vector<std::string> LoadLines(const std::string &path);
[[nodiscard]] std::vector<std::uint16_t> LinesToUint16(const std::vector<std::string> &lines);

#endif //ADVENTOFCODE2021_SHARED_HPP
