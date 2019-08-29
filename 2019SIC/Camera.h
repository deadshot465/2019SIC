#pragma once
#include "ICollidable.h"
#include "IMovable.h"

namespace ecc {
	class Camera :
		public IMovable, public ICollidable
	{
	public:
		// IMovable ����Čp������܂���
		virtual void Move() override;

		// ICollidable ����Čp������܂���
		virtual void SetCollisionBox() override;
		virtual void SetCollisionBox(int xLocation, int yLocation);
	};
}

