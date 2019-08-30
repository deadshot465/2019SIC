#pragma once
#include <SDL.h>
#include <string>
#include "Helper.h"

namespace ecc {

	class Character;
	class Enemy;
	class GameEngine;
	class Object;

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

		friend Character;
		friend Enemy;
		friend GameEngine;
		friend Object;
	public:
		Image();
		~Image();

		void MoveDestinationLocation(float x, float y) noexcept;
		void SetTexCoord(int x, int y) noexcept;

		void Render(SDL_Renderer* renderer, SDL_RendererFlip flipMode = SDL_FLIP_NONE, double angle = 0.0,
			const SDL_Point* center = nullptr);
	};
}

