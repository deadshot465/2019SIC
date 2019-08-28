#pragma once
#include <stdexcept>
#include <string>

namespace ecc {

	constexpr int TILE_WIDTH = 16;
	constexpr int TILE_HEIGHT = 16;
	constexpr int MAX_MAP_X = 32;
	constexpr int MAX_MAP_Y = 16;
	constexpr int MAX_TILE_COUNT_X = 15;
	constexpr int MAX_TILE_COUNT_Y = 2;

	inline void ThrowIfFailed(const std::string& message) {
		throw std::runtime_error(message);
	}
}