#pragma once
#include <chrono>
#include <vector>

namespace ecc {
	class IAnimatable
	{
	protected:
		std::vector<unsigned int> m_frameCounts;
		std::vector<double> m_animationSpeeds;
		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_currentTime;
		unsigned int m_currentFrame = 0;
		unsigned int m_currentXPos = 0;

		virtual void Animate(float speedFactor) = 0;
	};
}
