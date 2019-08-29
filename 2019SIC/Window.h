#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include "GameEngine.h"

namespace ecc {
	class GameWindow {
	private:
		SDL_Window* m_window = nullptr;
		SDL_Surface* m_surface = nullptr;
		SDL_Event m_event = {};

		std::unique_ptr<GameEngine> m_graphicsEngine = nullptr;

		bool m_isInit = false;
		float m_scaleX = 1.0f;
		float m_scaleY = 1.0f;
	public:
		GameWindow(const std::string& title, int width, int height,
			bool fullScreen = false,
			int xPos = SDL_WINDOWPOS_CENTERED, int yPos = SDL_WINDOWPOS_CENTERED);
		~GameWindow();

		void Broadcast();

		bool IsInit() const noexcept;
	};
}

