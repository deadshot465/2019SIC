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
#include "Tile.h"

namespace ecc {
	class Character : public IMovable, public IAnimatable, public ICollidable, public IRenderable
	{
	public:

		enum class CharacterFlag {
			Father,
			Daughter
		};

		Character(SDL_Renderer* renderer,
			const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			const std::string& jumpAnimationFileName,
			const std::string& fallAnimationFileName,
			const std::string& climbAnimationFileName,
			CharacterFlag characterFlag,
			int xPos, int yPos, float speed,
			float idleSpeed, float moveSpeed, float attackSpeed, float jumpSpeed, float fallSpeed,
			float climbSpeed,
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
		void SetAttackCollisionBox();
		const SDL_Rect& GetAttackCollisionBox() const noexcept;
		bool CheckAttackCollision(const SDL_Rect& other) const noexcept;

		// IRenderable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual const SDL_Rect& GetCurrentDestination() override;

		ImageIndexFlag GetCurrentStatus() const noexcept;

		void Jump();
		void Climb(int xPos, int yPos);
		void Fall();
	private:
		CharacterStatusFlag m_statusFlag = {};
		ImageIndexFlag m_currentImageIndex = ImageIndexFlag::Idle;
		CharacterFlag m_characterFlag = {};
		SDL_Rect m_attackCollisionBox = {};

		bool m_animationStarted = false;
		unsigned int m_animationTimer = 0;

		int m_jumpTimer = 10;
		int m_jumpSpeed = 32;
		bool m_jumpStarted = false;
		int m_jumpStartedYCoordinate = 0;
		bool m_fallStarted = false;
		bool m_climbUpStarted = false;
		bool m_climbDownStarted = false;
		int m_climbSpeed = 8;
	};
}
