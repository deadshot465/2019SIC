#pragma once
#include <memory>
#include <SDL.h>
#include <vector>
#include "Image.h"

namespace ecc {
	class IRenderable
	{
	protected:
		std::unique_ptr<Image> m_image = nullptr;
		SDL_RendererFlip m_flipMode = SDL_FLIP_NONE;
		std::vector<std::unique_ptr<Image>> m_images;

	public:
		virtual ~IRenderable();

		virtual void Render(SDL_Renderer* renderer, float speedFactor) = 0;
		virtual const SDL_Rect& GetCurrentDestination() = 0;
	};
}
