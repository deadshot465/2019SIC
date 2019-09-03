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
#include "Key.h"
#include "ObjectFactory.h"
#include "SwitchableWindow.h"
#include "Tile.h"

namespace ecc {
	using TileCoordinatesSet = std::vector<std::vector<SDL_Rect>>;

	class SceneManager;

	class GameEngine
	{
	private:
		inline static const MapPair TILESET_FILENAME = {
			"texture/tileset/Background_ver.2 (copy).png",
			"texture/tileset/Foreground_ver.2.0.png"
		};
		
		inline static const CharacterAnimationSet FATHER_ANIMATIONS = {
			"texture/animation/vampire_idle.png", "texture/animation/vampire_run.png",
			"texture/animation/vampire_slash.png", "texture/animation/vampire_jump(rise).png",
			"texture/animation/vampire_jump(fall).png", "texture/animation/vampire_climb.png",
		};

		inline static const CharacterAnimationSet DAUGHTER_ANIMATIONS = {
			"texture/animation/girl_idle (dark).png", "texture/animation/girl_walk(dark).png",
			"texture/animation/vampire_slash.png", "texture/animation/girl_jump(rise - dark).png",
			"texture/animation/girl_jump(fall - dark).png", "texture/animation/girl_climb(dark).png",
		};

		inline static const CharacterAnimationParameters FATHER_PARAMETERS = {
			1.25f, 0.25f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		inline static const CharacterAnimationParameters DAUGHTER_PARAMETERS = {
			1.5f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f
		};

		SDL_Renderer* m_renderer = nullptr;
		SDL_Texture* m_defaultRtv = nullptr;
		SDL_Texture* m_tileRtv = nullptr;
		SDL_Texture* m_cameraRtv = nullptr;

		std::unique_ptr<Camera> m_camera = nullptr;
		std::vector<std::unique_ptr<Image>> m_images;
		std::unique_ptr<Character> m_father = nullptr;
		std::unique_ptr<Character> m_daughter = nullptr;
		std::vector<std::unique_ptr<Enemy>> m_enemies;
		std::unique_ptr<ObjectFactory> m_objectFactory = nullptr;
		
		std::vector<TileCoordinatesSet> m_tileCoordinates;
		std::vector<std::vector<std::vector<int>>> m_maps;
		std::vector<std::shared_ptr<Tile>> m_tiles;
		std::vector<std::vector<std::shared_ptr<Tile>>> m_tileSet;
		std::vector<SDL_Rect> m_walkableTiles;
		std::vector<SDL_Rect> m_blockedTiles;
		std::vector<SDL_Rect> m_climbableTiles;
		std::map<size_t, size_t> m_tileCoordinateMapping;
		int m_currentMapWidth = 0;
		int m_currentMapHeight = 0;
		TileCoordinatesSet CalculateTileCoordinates(int totalWidth, int totalHeight);
		void GetAllWalkableTiles();
		void GetAllBlockedTiles();
		void GetAllClimbableTiles();

		unsigned short m_characterIndex = 0;

		std::vector<std::unique_ptr<SwitchableWindow>> m_switchableWindows;
		std::vector<std::unique_ptr<Key>> m_keys;

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

		bool m_windowGenerated = false;
		void GenerateWindows(Scene scene);
		void GenerateKeyAndDoors();

		void ClearScene();
		void LoadSprite(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier);

		void LoadMap(const std::string& mapName);
		void LoadTileSet(SDL_Surface* surface);
		void LoadCharacter(Character::CharacterFlag characterFlag, int xPos = 0, int yPos = 0,
			float speed = 5.0f, ImageIndexFlag initialStatus = ImageIndexFlag::Idle);

		GameStatus m_currentGameStatus = GameStatus::Normal;
		const GameStatus& GetCurrentGameStatus() const noexcept;

		friend SceneManager;
	public:
		GameEngine(SDL_Window* window, SDL_Surface* windowSurface);
		~GameEngine();
		
		void LoadEnemy(const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			int xPos, int yPos, float speed, int moveRange);

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render(SDL_Surface* windowSurface, Scene scene);

		void SwitchLight();
		void SetCharacterIndex();

		unsigned short GetCharacterIndex() const noexcept;
	};
}
