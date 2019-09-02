#pragma once
#include <SDL.h>

namespace ecc {
	class ICollidable
	{
	protected:
		SDL_Rect m_collisionBox = {};

		virtual void SetCollisionBox() = 0;
	public:
		virtual bool CheckCollision(const SDL_Rect& other);

		const SDL_Rect& GetCollisionBox() const noexcept;
	};
}
