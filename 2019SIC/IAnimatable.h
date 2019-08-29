#pragma once
#include <vector>

namespace ecc {
	class IAnimatable
	{
	protected:
		std::vector<unsigned int> m_frameCounts;

		virtual void Animate() = 0;
	};
}

