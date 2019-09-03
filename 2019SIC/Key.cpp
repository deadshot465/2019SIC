#include "Key.h"

ecc::Key::Key(SDL_Renderer* renderer, const std::string& animationFileName, int xPos, int yPos, double animationSpeed)
{
	
}

ecc::Key::~Key()
{
}

void ecc::Key::Animate(float speedFactor)
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	auto frame_duration =
		(static_cast<float>(m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) / 60) * m_animationSpeeds[static_cast<size_t>(m_currentImageIndex)];

	auto elapsed_time =
		std::chrono::duration<float,
		std::chrono::seconds::period>(m_currentTime - m_startTime).count();
}

void ecc::Key::Render(SDL_Renderer* renderer, float speedFactor)
{
}

const SDL_Rect& ecc::Key::GetCurrentDestination()
{
	return SDL_Rect();
}
