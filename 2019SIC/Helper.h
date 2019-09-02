#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace ecc {
	constexpr int TILE_WIDTH = 16;
	constexpr int TILE_HEIGHT = 16;
	constexpr int CHARACTER_SPRITE_WIDTH = 64;
	constexpr int CHARACTER_SPRITE_HEIGHT = 64;
	constexpr int CHARACTER_ATTACK_SPRITE_WIDTH = 80;
	constexpr int CHARACTER_ATTACK_SPRITE_HEIGHT = 64;
	constexpr int ENEMY_SPRITE_OFFSET = 3;

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

	constexpr int MAX_ANIMATION_COUNT = 5;

	enum class TileType {
		Normal,
		Switch
	};

	enum class ImageIndexFlag {
		Idle,
		Move,
		Attack,
		Jump,
		Fall
	};

	enum class CharacterStatusFlag {
		Incontrollable,
		Controllable
	};

	inline void ThrowIfFailed(const std::string& message) {
		throw std::runtime_error(message);
	}

	template <typename T = std::mt19937, std::size_t N = T::state_size>
	inline auto GetRandomSeededEngine() -> typename std::enable_if<!!N, T>::type
	{
		typename T::result_type random_data[N];
		std::random_device rd;
		std::generate(std::begin(random_data), std::end(random_data), std::ref(rd));
		std::seed_seq seeds(std::begin(random_data), std::end(random_data));
		T engine(seeds);
		return engine;
	}

	inline int GetRandomInt(int lower, int upper) {
		auto engine = GetRandomSeededEngine();
		auto generator = std::uniform_int_distribution<>(lower, upper);

		return generator(engine);
	}
}