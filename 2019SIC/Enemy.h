#pragma once
#include "IAnimatable.h"
#include "ICollidable.h"
#include "IMovable.h"
#include "IRenderable.h"

namespace ecc {
	class Enemy :
		public IAnimatable, public IMovable, public ICollidable, public IRenderable
	{
	private:
		float m_enemyRushSpeed = 0.0f;
		bool m_moveFinished = true;
		int m_nextDestination = 1;
		int m_targetDestination = 0;
		int m_enemyMoveRange = 0;
		int m_originalX = 0;
		ImageIndexFlag m_currentImageIndex = ImageIndexFlag::Idle;

	public:
		Enemy(SDL_Renderer* renderer,
			const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			const std::string& attackAnimationFileName,
			int xPos, int yPos, float speed,
			double idleSpeed, double moveSpeed, double attackSpeed,
			int moveRange);
		~Enemy();

		// IMovable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Move() override;
		void Move(const SDL_Rect& playerPosition, bool foundPlayer, SDL_Surface* windowSurface);
		void Attack();

		// ICollidable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void SetCollisionBox() override;

		// IRenderable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Render(SDL_Renderer* renderer, float speedFactor = 1.0f) override;

		// IAnimatable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Animate(float speedFactor = 1.0f) override;

		// IRenderable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual const SDL_Rect& GetCurrentDestination() override;

		void StartMove() noexcept;
		bool IsMoveFinished() const noexcept;
	};
}
