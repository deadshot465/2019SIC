#pragma once
#include <array>
#include <memory>
#include <SDL.h>
#include <string>
#include <map>
#include <vector>
#include "Helper.h"
#include "Image.h"
#include "Tile.h"

namespace ecc {

	using TileCoordinatesSet = std::array<std::array<SDL_Rect, MAX_TILE_COUNT_X>, MAX_TILE_COUNT_Y>;

	class GraphicsEngine
	{
	private:
		SDL_Renderer* m_renderer = nullptr;

		std::vector<std::unique_ptr<Image>> m_images;
		std::vector<TileCoordinatesSet> m_tileCoordinates;
		
		std::vector<unsigned int> m_texCoordIndices;
		std::vector<unsigned int> m_switchIndices;

		std::map<unsigned int, unsigned int> m_tileCoordinateSet;
		
		std::vector<int> m_map;
		std::vector<std::unique_ptr<Tile>> m_tiles;
		TileCoordinatesSet CalculateTileCoordinates();
		
		void CreateTiles(unsigned int imageIndex);
		void RenderTiles();
		Tile* GetTile(const SDL_Rect& location) noexcept;
	public:
		GraphicsEngine(SDL_Window* window);
		~GraphicsEngine();

		void LoadImage(const std::string& fileName, bool isTileSet = false);
		void LoadMap(const std::string& mapName);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render();

		void SwitchLight();
	};
}

