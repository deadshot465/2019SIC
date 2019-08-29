#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include "IAnimatable.h"
#include "Image.h"
#include "IMovable.h"

namespace ecc {
	class Character : public IMovable, public IAnimatable
	{
	private:
		std::unique_ptr<Image> m_image = nullptr;

	public:
		Character(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, float speed = 5.0f);
		~Character();

		virtual void Move() override;
		void Render(SDL_Renderer* renderer);

		// ìßâﬂ IAnimatable „ãè≥
		virtual void Animate() override;
	};
}

