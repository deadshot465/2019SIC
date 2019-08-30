#pragma once
#include <array>
#include <memory>
#include <SDL.h>
#include <string>
#include <map>
#include <vector>
#include "Camera.h"
#include "Character.h"
#include "Enemy.h"
#include "Helper.h"
#include "Image.h"
#include "Object.h"
#include "Tile.h"

namespace ecc {

	using TileCoordinatesSet = std::vector<std::vector<SDL_Rect>>;

	class GameEngine
	{
	private:
		SDL_Renderer* m_renderer = nullptr;
		SDL_Texture* m_defaultRtv = nullptr;
		SDL_Texture* m_tileRtv = nullptr;
		std::vector<SDL_Texture*> m_tileRtvs;

		std::unique_ptr<Camera> m_camera = nullptr;
		std::vector<std::unique_ptr<Image>> m_images;
		std::unique_ptr<Character> m_father = nullptr;
		std::unique_ptr<Character> m_daughter = nullptr;
		std::vector<std::unique_ptr<Enemy>> m_enemies;
		std::vector<std::unique_ptr<Object>> m_objects;
		std::vector<TileCoordinatesSet> m_tileCoordinates;
		
		std::vector<size_t> m_texCoordIndices;
		std::vector<size_t> m_switchIndices;

		std::map<size_t, size_t> m_tileCoordinateMapping;
		
		std::vector<int> m_map;
		std::vector<std::vector<std::vector<int>>> m_maps;
		int m_currentMapWidth = 0;
		int m_currentMapHeight = 0;
		std::vector<std::shared_ptr<Tile>> m_tiles;
		std::vector<std::vector<std::shared_ptr<Tile>>> m_tileSet;
		TileCoordinatesSet CalculateTileCoordinates(int totalWidth, int totalHeight);

		bool m_tileRendered = false;
		unsigned short m_characterIndex = 0;
		
		void CreateTiles(size_t imageIndex, int totalWidth, int totalHeight);
		void RenderTiles(SDL_Surface* windowSurface);
		Tile* GetTile(const SDL_Rect& location) noexcept;
		
		int GetSingleDirectionMoveBound(const SDL_Rect& nextArea, int increment);
		void GetEnemyMoveBounds(Enemy* enemy, int& leftBound, int& rightBound);
		void MoveEnemy();
	public:
		GameEngine(SDL_Window* window);
		~GameEngine();

		void LoadImage(const std::string& fileName, bool isTileSet = false);
		void LoadObject(const std::string& fileName, int xPos, int yPos);
		void LoadMap(const std::string& mapName, const std::string& fileName);
		void LoadCharacter(const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			Character::CharacterFlag characterFlag,
			int xPos = 0, int yPos = 0, float speed = 5.0f,
			ImageIndexFlag initialStatus = ImageIndexFlag::Idle);
		void LoadEnemy(const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			int xPos = 0, int yPos = 0, float speed = 40.0f);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render(SDL_Surface* windowSurface, float scaleX, float scaleY);

		void SwitchLight();
		void SetCharacterIndex(unsigned short index);

		unsigned short GetCharacterIndex() const noexcept;
	};
}

