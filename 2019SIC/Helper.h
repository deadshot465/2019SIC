#pragma once
#include <stdexcept>
#include <string>

namespace ecc {

	constexpr int TILE_WIDTH = 16;
	constexpr int TILE_HEIGHT = 16;
	constexpr int CHARACTER_SPRITE_WIDTH = 64;
	constexpr int CHARACTER_SPRITE_HEIGHT = 64;
	
	constexpr int MAX_MAP_X = 32;
	constexpr int MAX_MAP_Y = 16;
	constexpr int MAP_WIDTH = MAX_MAP_X * TILE_WIDTH;
	constexpr int MAP_HEIGHT = MAX_MAP_Y * TILE_HEIGHT;

	constexpr int MAX_TILE_COUNT_X = 10;
	constexpr int MAX_TILE_COUNT_Y = 2;

	constexpr int TRANSPARENT_R = 0x99;
	constexpr int TRANSPARENT_G = 0xE5;
	constexpr int TRANSPARENT_B = 0x50;

	constexpr int LEFT_X_BOUND = TILE_WIDTH / 4;
	constexpr int RIGHT_X_BOUND = MAX_MAP_X * TILE_WIDTH - (4 * TILE_WIDTH);
	constexpr int TOP_Y_BOUND = TILE_HEIGHT;
	constexpr int BOTTOM_Y_BOUND = MAX_MAP_Y * TILE_HEIGHT - TILE_HEIGHT;

	constexpr int MAX_ANIMATION_COUNT = 2;

	enum class TileType {
		Normal,
		Switch
	};

	inline void ThrowIfFailed(const std::string& message) {
		throw std::runtime_error(message);
	}
}