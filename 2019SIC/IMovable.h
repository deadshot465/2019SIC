#pragma once

namespace ecc {
	class IMovable
	{
	protected:
		float m_speed = 0.0f;

	public:
		virtual void Move() = 0;
	};
}

