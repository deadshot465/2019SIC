#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include "IAnimatable.h"
#include "ICollidable.h"
#include "Image.h"
#include "IMovable.h"

namespace ecc {
	class Character : public IMovable, public IAnimatable, public ICollidable
	{
	private:
		std::vector<std::unique_ptr<Image>> m_images;

		unsigned short m_currentImageIndex = 0;

	public:
		Character(SDL_Renderer* renderer,
			const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			int xPos, int yPos, float speed = 5.0f);
		~Character();

		virtual void Move() override;
		void Render(SDL_Renderer* renderer);
		void SetStatus(unsigned short status);

		// ìßâﬂ IAnimatable „ãè≥
		virtual void Animate() override;

		// ICollidable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void SetCollisionBox() override;

		const SDL_Rect& GetCurrentDestination() const noexcept;
	};
}

