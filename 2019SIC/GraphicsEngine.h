#pragma once
#include <array>
#include <memory>
#include <SDL.h>
#include <string>
#include <map>
#include <vector>
#include "Character.h"
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
		std::vector<std::unique_ptr<Character>> m_characters;
		std::vector<TileCoordinatesSet> m_tileCoordinates;
		
		std::vector<size_t> m_texCoordIndices;
		std::vector<size_t> m_switchIndices;

		std::map<size_t, size_t> m_tileCoordinateSet;
		
		std::vector<int> m_map;
		std::vector<std::unique_ptr<Tile>> m_tiles;
		TileCoordinatesSet CalculateTileCoordinates();
		
		void CreateTiles(size_t imageIndex);
		void RenderTiles();
		Tile* GetTile(const SDL_Rect& location) noexcept;
	public:
		GraphicsEngine(SDL_Window* window);
		~GraphicsEngine();

		void LoadImage(const std::string& fileName, bool isTileSet = false);
		void LoadMap(const std::string& mapName);
		void LoadCharacter(const std::string& fileName, int xPos = 0, int yPos = 0, float speed = 5.0f);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a, float scaleX, float scaleY);
		void Render();

		void SwitchLight();
	};
}

