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
			const std::string& jumpAnimationFileName,
			const std::string& fallAnimationFileName,
			CharacterFlag characterFlag,
			int xPos, int yPos, float speed,
			float idleSpeed, float moveSpeed, float attackSpeed, float jumpSpeed, float fallSpeed,
			ImageIndexFlag initialStatus);
		~Character();

		virtual void Move() override;
		void Move(SDL_Surface* windowSurface);
		virtual void Render(SDL_Renderer* renderer, float speedFactor = 1.0f) override;
		void Render(SDL_Renderer* renderer, int offsetX, int offsetY, float speedFactor = 1.0f);
		void SetStatus(CharacterStatusFlag status);

		// IAnimatable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void Animate(float speedFactor) override;

		// ICollidable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void SetCollisionBox() override;

		// IRenderable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual const SDL_Rect& GetCurrentDestination() override;

		ImageIndexFlag GetCurrentStatus() const noexcept;

		void Jump();

	private:
		CharacterStatusFlag m_statusFlag = {};
		ImageIndexFlag m_currentImageIndex = ImageIndexFlag::Idle;
		CharacterFlag m_characterFlag = {};

		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_currentTime;
		unsigned int m_currentFrame = 0;
		unsigned int m_currentXPos = 0;
		bool m_animationStarted = false;
		unsigned int m_animationTimer = 0;
		std::vector<float> m_animationSpeeds;

		int m_jumpTimer = 1.0;
		int m_jumpSpeed = 32;
		bool m_jumpStarted = false;
		int m_jumpStartedYCoordinate = 0;
		bool m_fallStarted = false;
	};
}

