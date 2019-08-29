#pragma once
#include <memory>
#include "ICollidable.h"
#include "Image.h"

namespace ecc {
	class Object : public ICollidable
	{
	private:
		std::unique_ptr<Image> m_image = nullptr;

	public:
		Object(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos);
		~Object();

		// ICollidable ����Čp������܂���
		virtual void SetCollisionBox() override;

		void Render(SDL_Renderer* renderer);
	};
}

