#pragma once
#include "ICollidable.h"
#include "IMovable.h"

namespace ecc {
	class Camera :
		public IMovable, public ICollidable
	{
	public:
		// IMovable を介して継承されました
		virtual void Move() override;

		// ICollidable を介して継承されました
		virtual void SetCollisionBox() override;
		virtual void SetCollisionBox(int xLocation, int yLocation);
	};
}

