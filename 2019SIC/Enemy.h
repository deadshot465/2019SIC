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
		static const int m_enemyInitialSpeed = 1;
		int m_enemyCurrentSpeed = 0;
		bool m_moveFinished = true;
		int m_nextDestination = 0;

	public:
		Enemy(SDL_Renderer* renderer,
			const std::string& waitAnimationFileName,
			const std::string& moveAnimationFileName,
			int xPos, int yPos, float speed);
		~Enemy();

		// IMovable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Move() override;
		void Move(int nextDestination);
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

