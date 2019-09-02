#pragma once
#include "ICollidable.h"
#include "IMovable.h"

namespace ecc {
	class Camera :
		public IMovable, public ICollidable
	{
	private:
		SDL_Surface* m_surfaceHandle = nullptr;
	public:
		Camera(SDL_Surface* windowSurface);
		~Camera();

		// IMovable ����Čp������܂���
		virtual void Move() override;

		// ICollidable ����Čp������܂���
		virtual void SetCollisionBox() override;
		virtual void SetCollisionBox(int xLocation, int yLocation, int maxX, int maxY);
	};
}
