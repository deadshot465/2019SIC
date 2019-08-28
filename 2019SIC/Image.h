#pragma once
#include <SDL.h>
#include <string>

namespace ecc {

	class GraphicsEngine;

	class Image
	{
	private:
		SDL_Surface* m_image = nullptr;
		SDL_Texture* m_texture = nullptr;
		int m_width = 0;
		int m_height = 0;

		void LoadImage(SDL_Renderer* renderer, const std::string& fileName);

		friend GraphicsEngine;
	public:
		Image();
		~Image();

		SDL_Texture* GetTexture() const noexcept;
		SDL_Rect GetRect() const noexcept;
	};
}

