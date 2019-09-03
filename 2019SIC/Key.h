#pragma once
#include "IAnimatable.h"
#include "IRenderable.h"

namespace ecc {
	class Key :
		public IAnimatable, public IRenderable
	{
	public:
		Key(SDL_Renderer* renderer, const std::string& animationFileName, int xPos, int yPos,
			double animationSpeed);
		~Key();

		// IAnimatable を介して継承されました
		virtual void Animate(float speedFactor) override;

		// IRenderable を介して継承されました
		virtual void Render(SDL_Renderer* renderer, float speedFactor) override;
		virtual const SDL_Rect& GetCurrentDestination() override;
	};
}

