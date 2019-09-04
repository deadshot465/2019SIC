#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include "GameEngine.h"
#include "Helper.h"

namespace ecc {

	class SceneManager
	{
	private:
		inline static const MapPair STAGE_HALL1_FILENAME = {
			"map/stage1_hall_background.txt", "map/stage1_hall_foreground.txt"
		};
		inline static const MapPair STAGE_HALL2_FILENAME = {
			"map/stage_hall2_background.txt", "map/stage_hall2_foreground.txt"
		};
		inline static const MapPair STAGE_HALL3_FILENAME = {
			"map/Stage_hall3_background.txt", "map/Stage_hall3_foreground.txt"
		};
		inline static const MapPair STAGE_1_FILENAME = {
			"map/Stage1_background.txt", "map/Stage1_foreground.txt"
		};
		inline static const MapPair STAGE_2_FILENAME = {
			"map/Stage2_background.txt", "map/Stage2_foreground.txt"
		};
		inline static const MapPair STAGE_3_FILENAME = {
			"map/Stage3_background.txt", "map/Stage3_foreground.txt"
		};
		
		inline static const std::string TITLE_SCREEN_FILENAME = "texture/sprite/cover1.png";
		inline static const std::string GAMEOVER_SCREEN_FILENAME = "texture/sprite/gameover.png";
		inline static const std::string STATIC_BACKGROUND_FILENAME = "texture/sprite/final bg.png";
		inline static const std::string DYNAMIC_BACKGROUND_FILENAME = "texture/sprite/final bg.png";
		inline static const std::string HP_BAR_FILENAME = "texture/sprite/healthbar.png";
		inline static const std::string HP_CHUNK_FILENAME = "texture/sprite/HP chunk.png";
		inline static const std::string DEFENSE_FILENAME = "map/Stage_DefenceFG.txt";

		std::unique_ptr<GameEngine> m_gameEngine = nullptr;
		
	public:

		SceneManager(SDL_Window* window, SDL_Surface* surface);
		~SceneManager();

		void LoadScene(SDL_Window* window, SDL_Surface** surface, Scene scene);
		void Render(SDL_Surface* surface, float scaleX, float scaleY);

		const Scene& GetCurrentScene() const noexcept;
		const GameStatus& GetCurrentGameStatus() const noexcept;

	private:
		Scene m_currentScene = Scene::None;
	};
}

