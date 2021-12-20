#include "Image.hpp"

#include <utility>
#include <bitset>

Image::Image(std::vector<bool> pixels, size_t width, size_t height)
		: pixels(std::move(pixels)), width(width), height(height)
{}

void Image::Pad(std::size_t size)
{
	const std::size_t newWidth = width + (2 * size), newHeight = height + (2 * size);

	std::vector<bool> newPixels(newWidth * newHeight, false);
	for (auto i = 0U; i < height; ++i)
	{
		const auto source = std::next(pixels.begin(), i * width);
		const auto destination = std::next(newPixels.begin(), (i + size) * newWidth + size);
		std::copy_n(source, width, destination);
	}

	width = newWidth;
	height = newHeight;
	pixels = std::move(newPixels);
}

std::ostream &operator<<(std::ostream &os, const Image &image)
{
	for (auto y = 0U; y < image.height; ++y)
	{
		for (auto x = 0U; x < image.width; ++x)
		{
			os << (image.pixels[y * image.width + x] ? '#' : '.');
		}
		os << "\r\n";
	}
	return os;
}

std::array<Image::Position, 9U> Image::GetSquare(const Image::Position &currentPosition)
{
	static constinit const std::array<Position, 9U> steps =
			{
					std::make_pair(-1, -1),
					std::make_pair(0, -1),
					std::make_pair(1, -1),
					std::make_pair(-1, 0),
					std::make_pair(0, 0),
					std::make_pair(1, 0),
					std::make_pair(-1, 1),
					std::make_pair(0, 1),
					std::make_pair(1, 1),
			};

	std::array<Position, 9U> result{};

	for (auto i = 0U; i < 9; ++i)
	{
		result[i] = {currentPosition.first + steps[i].first, currentPosition.second + steps[i].second};
	}

	return result;
}

void Image::Enhance(const Algorithm &algo)
{
	auto newPixels = pixels;

	for (auto y = 1U; y < height - 1; ++y)
	{
		for (auto x = 1U; x < width - 1; ++x)
		{
			const auto positions = GetSquare({x, y});

			std::bitset<9U> bits {};
			for (auto i = 0U; i < bits.size(); ++i)
			{
				bits[bits.size() - 1 - i] = pixels[positions[i].second * width + positions[i].first];
			}
			const auto algoIndex = bits.to_ulong();
			newPixels[y * width + x] = algo[algoIndex];
		}
	}

	pixels = std::move(newPixels);
}

std::uint64_t Image::Count() const
{
	return std::count(pixels.begin(), pixels.end(), true);
}

void Image::Strip(std::size_t size)
{
	const std::size_t newWidth = width - (2 * size), newHeight = height - (2 * size);

	std::vector<bool> newPixels {};
	newPixels.reserve(newWidth * newHeight);

	for (auto y = size; y <= height - size; ++y)
	{
		const auto source = std::next(pixels.begin(), y * width + size);
		std::copy_n(source, newWidth, std::back_inserter(newPixels));
	}

	width = newWidth;
	height = newHeight;
	pixels = std::move(newPixels);
}
