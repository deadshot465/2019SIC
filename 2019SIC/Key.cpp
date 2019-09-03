#include "Key.h"

ecc::Key::Key(SDL_Renderer* renderer, const std::string& animationFileName, int xPos, int yPos, double animationSpeed)
{	
	m_image = std::make_unique<Image>();
	m_image->LoadAnimation(renderer, animationFileName, xPos, yPos, 16, 16);

	m_frameCounts.emplace_back(m_image->m_width / TILE_WIDTH);

	m_animationSpeeds.emplace_back(animationSpeed);

	m_startTime = std::chrono::high_resolution_clock::now();

	SetCollisionBox();
}

ecc::Key::~Key()
{
	IRenderable::~IRenderable();
}

void ecc::Key::Animate(float speedFactor)
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	auto frame_duration =
		(static_cast<float>(m_frameCounts[0]) / 60) * m_animationSpeeds[0];

	auto elapsed_time =
		std::chrono::duration<float,
		std::chrono::seconds::period>(m_currentTime - m_startTime).count();

	if (elapsed_time > frame_duration) {
		m_currentXPos = m_currentFrame++ * TILE_WIDTH;

		if (m_currentFrame >= m_frameCounts[0]) {
			m_currentFrame = 0;
		}

		m_startTime = m_currentTime;
	}

	m_image->SetTexCoord(m_currentXPos, m_image->m_texCoord.y);
}

void ecc::Key::Render(SDL_Renderer* renderer, float speedFactor)
{
	if (!m_isActive) return;

	Animate(speedFactor);
	m_image->Render(renderer, m_flipMode);
}

const SDL_Rect& ecc::Key::GetCurrentDestination()
{
	return SDL_Rect();
}

void ecc::Key::SetCollisionBox()
{
	m_collisionBox.x = m_image->m_destinationLocation.x;
	m_collisionBox.y = m_image->m_destinationLocation.y;
	m_collisionBox.w = 16;
	m_collisionBox.h = 16;
}

void ecc::Key::SetActive(bool status) noexcept
{
	m_isActive = status;
}

bool ecc::Key::GetActiveStatus() const noexcept
{
	return m_isActive;
}
