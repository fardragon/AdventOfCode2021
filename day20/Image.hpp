#ifndef ADVENTOFCODE2021_IMAGE_HPP
#define ADVENTOFCODE2021_IMAGE_HPP

#include <cstdint>
#include <vector>
#include <ostream>
#include <array>

using Algorithm = std::array<bool, 512>;
class Image
{
public:
	Image(std::vector<bool> pixels, size_t width, size_t height);

	void Pad(std::size_t size);
	void Strip(std::size_t size);
	void Enhance(const Algorithm &algo);
	std::uint64_t Count() const;

private:
	using Position = std::pair<std::size_t, std::size_t>;
	friend std::ostream &operator<<(std::ostream &os, const Image &image);
	std::array<Image::Position, 9U> GetSquare(const Position &currentPosition);

private:
	std::vector<bool> pixels;
	std::size_t width;
	std::size_t height;
};

std::ostream &operator<<(std::ostream &os, const Image &image);


#endif //ADVENTOFCODE2021_IMAGE_HPP
