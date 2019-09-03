#pragma once
#include "IAnimatable.h"
#include "ICollidable.h"
#include "IRenderable.h"

namespace ecc {
	class Key :
		public IAnimatable, public IRenderable, public ICollidable
	{
	private:
		bool m_isActive = true;

	public:
		Key(SDL_Renderer* renderer, const std::string& animationFileName, int xPos, int yPos,
			double animationSpeed);
		~Key();

		// IAnimatable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Animate(float speedFactor) override;

		// IRenderable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void Render(SDL_Renderer* renderer, float speedFactor) override;
		virtual const SDL_Rect& GetCurrentDestination() override;

		// ICollidable ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void SetCollisionBox() override;
		
		void SetActive(bool status) noexcept;
		bool GetActiveStatus() const noexcept;
	};
}

