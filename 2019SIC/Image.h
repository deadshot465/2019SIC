#pragma once
#include <SDL.h>
#include <string>
#include "Helper.h"

namespace ecc {
	class Character;
	class Enemy;
	class GameEngine;
	class Key;
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

		void LoadImage(SDL_Renderer* renderer, const std::string& fileName, bool transparency = true, bool isTile = false);
		void LoadObject(SDL_Renderer* renderer, const std::string& fileName);
		void LoadAnimation(SDL_Renderer* renderer, const std::string& fileName,
			int xPos, int yPos, int texWidth, int texHeight);
		
		void LoadSprite(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, bool transparency = true, float magnifier = 1.0f);
		void LoadCharacterSprites(SDL_Renderer* renderer, const std::string& fileName,
			ecc::ImageIndexFlag imageFlag, int xPos, int yPos);

		friend Character;
		friend Enemy;
		friend GameEngine;
		friend Key;
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
