#pragma once
#include <SDL.h>
#include <string>

namespace ecc {

	class Character;
	class GraphicsEngine;

	class Image
	{
	private:
		SDL_Surface* m_image = nullptr;
		SDL_Texture* m_texture = nullptr;
		int m_width = 0;
		int m_height = 0;
		SDL_Rect m_texCoord = {};
		SDL_Rect m_destinationLocation = {};

		void LoadImage(SDL_Renderer* renderer, const std::string& fileName, bool transparency = true, int xPos = 0, int yPos = 0);

		friend Character;
		friend GraphicsEngine;
	public:
		Image();
		~Image();

		void MoveDestinationLocation(int x, int y) noexcept;
		void SetTexCoord(int x, int y) noexcept;

		SDL_Texture* GetTexture() const noexcept;
		const SDL_Rect& GetTexCoord() const noexcept;
		const SDL_Rect& GetDestinationLocation() const noexcept;
	};
}

