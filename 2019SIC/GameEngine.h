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
#include "ObjectFactory.h"
#include "SwitchableWindow.h"
#include "Tile.h"

namespace ecc {
	using TileCoordinatesSet = std::vector<std::vector<SDL_Rect>>;

	class GameEngine
	{
	private:
		SDL_Renderer* m_renderer = nullptr;
		SDL_Texture* m_defaultRtv = nullptr;
		SDL_Texture* m_tileRtv = nullptr;
		SDL_Texture* m_cameraRtv = nullptr;
		std::vector<SDL_Texture*> m_tileRtvs;

		std::unique_ptr<Camera> m_camera = nullptr;
		std::vector<std::unique_ptr<Image>> m_images;
		std::unique_ptr<Character> m_father = nullptr;
		std::unique_ptr<Character> m_daughter = nullptr;
		std::vector<std::unique_ptr<Enemy>> m_enemies;
		std::unique_ptr<ObjectFactory> m_objectFactory = nullptr;
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

		std::vector<std::unique_ptr<SwitchableWindow>> m_switchableWindows;

		void UpdateCharacters(int offsetX, int offsetY, SDL_Surface* windowSurface);

		void CreateTiles(size_t imageIndex, int totalWidth, int totalHeight,
			SDL_Surface* windowSurface);
		void RenderTiles(SDL_Surface* windowSurface, const SDL_Rect& cameraRect);
		Tile* GetBackgroundTile(const SDL_Rect& location) noexcept;
		Tile* GetForegroundTile(const SDL_Rect& location) noexcept;
		bool CheckMovableBetween(const SDL_Rect& src, const SDL_Rect& target, int increment = TILE_WIDTH * 2);

		int GetSingleDirectionMoveBound(const SDL_Rect& nextArea, int increment);
		void GetEnemyMoveBounds(Enemy* enemy, int& leftBound, int& rightBound);
		void MoveEnemy(SDL_Surface* windowSurface);
		
		void GenerateWindows();
		void GenerateKeyAndDoors();
	public:
		GameEngine(SDL_Window* window, SDL_Surface* windowSurface);
		~GameEngine();

		void LoadImage(const std::string& fileName, SDL_Surface* windowSurface,
			bool isTileSet = false);
		void LoadObject(const std::string& fileName, int xPos, int yPos);
		void LoadMap(const std::string& mapName, const std::string& fileName,
			SDL_Surface* windowSurface);
		void LoadCharacter(const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			const std::string& jumpAnimationFileName,
			const std::string& fallAnimationFileName,
			Character::CharacterFlag characterFlag,
			int xPos = 0, int yPos = 0, float speed = 5.0f,
			ImageIndexFlag initialStatus = ImageIndexFlag::Idle);
		void LoadEnemy(const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			int xPos, int yPos, float speed, int moveRange);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render(SDL_Surface* windowSurface, float scaleX, float scaleY);

		void SwitchLight();
		void SetCharacterIndex();

		unsigned short GetCharacterIndex() const noexcept;
	};
}
