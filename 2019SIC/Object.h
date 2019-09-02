#pragma once
#include <memory>
#include "ICollidable.h"
#include "Image.h"
#include "IRenderable.h"

namespace ecc {
	class Object : public ICollidable, public IRenderable
	{
	public:
		Object(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos);
		~Object();

		// ICollidable を介して継承されました
		virtual void SetCollisionBox() override;

		virtual void Render(SDL_Renderer* renderer, float speedFactor = 1.0f) override;

		// IRenderable を介して継承されました
		virtual const SDL_Rect& GetCurrentDestination() override;
	};
}
