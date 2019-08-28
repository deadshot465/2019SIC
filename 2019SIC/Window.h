#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include "GraphicsEngine.h"

namespace ecc {
	class GameWindow {
	private:
		SDL_Window* m_window = nullptr;
		SDL_Surface* m_surface = nullptr;
		SDL_Event m_event = {};

		std::unique_ptr<GraphicsEngine> m_graphicsEngine = nullptr;

		bool m_isInit = false;
	public:
		GameWindow(const std::string& title, int width, int height,
			bool fullScreen = false,
			int xPos = SDL_WINDOWPOS_CENTERED, int yPos = SDL_WINDOWPOS_CENTERED);
		~GameWindow();

		void Broadcast();

		bool IsInit() const noexcept;
	};
}

