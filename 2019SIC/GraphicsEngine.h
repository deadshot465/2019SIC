#pragma once
#include <array>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include "Helper.h"
#include "Image.h"

namespace ecc {
	class GraphicsEngine
	{
	private:
		SDL_Renderer* m_renderer = nullptr;

		std::vector<std::unique_ptr<Image>> m_images;
		std::array<std::array<SDL_Rect, MAX_TILE_COUNT_X>, MAX_TILE_COUNT_Y> m_tileCoordinates;
		std::vector<int> m_map;

		void CalculateTileCoordinates();
	public:
		GraphicsEngine(SDL_Window* window);
		~GraphicsEngine();

		void LoadImage(const std::string& fileName);
		void LoadMap(const std::string& mapName);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render();
	};
}

