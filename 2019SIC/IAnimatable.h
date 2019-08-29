#pragma once

namespace ecc {
	class IAnimatable
	{
	protected:
		unsigned int m_frameCount = 0;

		virtual void Animate() = 0;
	};
}

