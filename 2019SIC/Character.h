#pragma once
#include <chrono>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include "IAnimatable.h"
#include "ICollidable.h"
#include "Image.h"
#include "IMovable.h"
#include "IRenderable.h"

namespace ecc {
	class Character : public IMovable, public IAnimatable, public ICollidable, public IRenderable
	{
	public:

		static enum class CharacterFlag {
			Father,
			Daughter
		};

		Character(SDL_Renderer* renderer,
			const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			CharacterFlag characterFlag,
			int xPos, int yPos, float speed, ImageIndexFlag initialStatus);
		~Character();

		virtual void Move() override;
		virtual void Render(SDL_Renderer* renderer, float speedFactor = 1.0f) override;
		void Render(SDL_Renderer* renderer, int offsetX, int offsetY, float speedFactor = 1.0f);
		void SetStatus(ImageIndexFlag status);

		// IAnimatable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Animate(float speedFactor) override;

		// ICollidable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void SetCollisionBox() override;

		// IRenderable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual const SDL_Rect& GetCurrentDestination() override;

		ImageIndexFlag GetCurrentStatus() const noexcept;

	private:
		ImageIndexFlag m_currentImageIndex = ImageIndexFlag::Idle;
		CharacterFlag m_characterFlag = {};

		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_currentTime;
		unsigned int m_currentFrame = 0;
		unsigned int m_currentXPos = 0;
	};
}

