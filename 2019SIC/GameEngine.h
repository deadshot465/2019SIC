#pragma once
#include <array>
#include <list>
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
#include "Mixer.h"
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
			"texture/animation/girl_idle.png", "texture/animation/girl_walk.png",
			"texture/animation/vampire_slash.png", "texture/animation/girl_jump(rise).png",
			"texture/animation/girl_jump(fall).png", "texture/animation/girl_climb.png",
		};

		inline static const EnemyAnimationSet ENEMY_ANIMATIONS = {
			"texture/animation/vampire_enemy_idle.png", "texture/animation/vampire_enemy_walk.png",
			"texture/animation/vampire_enemy_run.png"
		};

		inline static const CharacterAnimationParameters FATHER_PARAMETERS = {
			1.25f, 0.25f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		inline static const CharacterAnimationParameters DAUGHTER_PARAMETERS = {
			1.5f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f
		};

		inline static const EnemyAnimationParameters ENEMY_PARAMETERS = {
			4.5f, 3.0f, 0.45f
		};

		std::vector<Vector2> RandomEnemyPositions = {
			{16, 0}, {1584, 0}, {16, 448}, {1584, 448}, {16, 640}, {1584, 640}
		};

		SDL_Renderer* m_renderer = nullptr;
		SDL_Texture* m_defaultRtv = nullptr;
		SDL_Texture* m_tileRtv = nullptr;
		SDL_Texture* m_cameraRtv = nullptr;

		std::vector<std::unique_ptr<Image>> m_images;
		std::list<std::unique_ptr<Enemy>> m_enemies;
		std::unique_ptr<Camera> m_camera = nullptr;
		std::unique_ptr<Character> m_father = nullptr;
		std::unique_ptr<Character> m_daughter = nullptr;		
		std::unique_ptr<ObjectFactory> m_objectFactory = nullptr;
		std::unique_ptr<Mixer> m_mixer = nullptr;
		
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

		std::unique_ptr<Image> m_animatedSprite;
		void ClearScene();
		void LoadSprite(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier);
		void LoadAnimatedSprite(const std::string& fileName, int xPos, int yPos, bool transparency,
			float magnifier, int clipX, int clipY);
		void PlayAnimatedSprite();

		void LoadMap(const std::string& mapName);
		void LoadTileSet(SDL_Surface* surface, bool loadBackground = true, bool loadForeground = true);
		void LoadCharacter(Character::CharacterFlag characterFlag, int xPos = 0, int yPos = 0,
			float speed = 5.0f, ImageIndexFlag initialStatus = ImageIndexFlag::Idle);
		void LoadEnemy(int xPos, int yPos, float speed);

		GameStatus m_currentGameStatus = GameStatus::Normal;
		const GameStatus& GetCurrentGameStatus() const noexcept;

		std::vector<SDL_Rect> m_collisionBoxes;
		void CreateCollisionBox(int xPos, int yPos, int width, int height);

		void GenerateEnemy();

		int m_hp = 500;
		std::unique_ptr<Image> m_hpBar = nullptr;
		std::vector<std::unique_ptr<Image>> m_hpChunk;
		void LoadHpBar(const std::string& fileName, int xPos, int yPos,
			bool transparency, float magnifier);
		void LoadHpChunk(const std::string& fileName, int xPos, int yPos,
			bool transparency, float magnifier);

		int m_combo = 0;
		std::unique_ptr<Image> m_comboSprite = nullptr;
		std::unique_ptr<Image> m_comboWordSprite = nullptr;
		void LoadComboSprite(const std::string& numberFileName,
			const std::string& comboFileName,
			int xPos, int yPos,
			bool transparency, float magnifier, int clipX, int clipY);
		void SwitchComboSprite();

		friend SceneManager;
	public:
		GameEngine(SDL_Window* window, SDL_Surface* windowSurface);
		~GameEngine();

		void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Render(SDL_Surface* windowSurface, Scene scene);

		void SwitchLight();
		void SetCharacterIndex();

		unsigned short GetCharacterIndex() const noexcept;
	};
}
