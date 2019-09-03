#pragma once
#include <SDL.h>
#include <string>
#include "Helper.h"

namespace ecc {
	class Character;
	class Enemy;
	class GameEngine;
	class ObjectFactory;

	class Image
	{
	private:
		SDL_Surface* m_image = nullptr;
		SDL_Texture* m_texture = nullptr;
		int m_width = 0;
		int m_height = 0;
		SDL_Rect m_texCoord = {};
		SDL_Rect m_destinationLocation = {};
		bool m_isTile = false;

		void LoadImage(SDL_Renderer* renderer, const std::string& fileName, bool transparency = true, int xPos = 0, int yPos = 0, bool isTile = false, ImageIndexFlag imageFlag = {});
		void LoadObject(SDL_Renderer* renderer, const std::string& fileName);

		friend Character;
		friend Enemy;
		friend GameEngine;
		friend ObjectFactory;
	public:
		Image();
		~Image();

		void MoveDestinationLocation(float x, float y) noexcept;
		void SetTexCoord(int x, int y) noexcept;

		void Render(SDL_Renderer* renderer, SDL_RendererFlip flipMode = SDL_FLIP_NONE, double angle = 0.0,
			const SDL_Point * center = nullptr, int offsetX = 0, int offsetY = 0);
		void RenderObject(SDL_Renderer* renderer, int xPos, int yPos);
	};
}
