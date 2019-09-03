#include "SwitchableWindow.h"

ecc::SwitchableWindow::SwitchableWindow(ObjectList onType, ObjectList offType, int xPos, int yPos,
	int xRange, int yRange,
	bool isLit, int group) :
	m_onType(onType), m_offType(offType), m_xPos(xPos), m_yPos(yPos),
	m_xLightingRange(xRange), m_yLightingRange(yRange),
	m_lit(isLit), m_group(group)
{
}

void ecc::SwitchableWindow::SetCollisionBox()
{
	
}

void ecc::SwitchableWindow::SetCollisionBox(ObjectFactory* factory)
{
	int width = 0;
	int height = 0;
	factory->GetImageDimension(m_onType, width, height);
	m_collisionBox.x = m_xPos + 2 * TILE_WIDTH;
	m_collisionBox.y = m_yPos + height;
	m_collisionBox.w = m_xLightingRange * 2 * TILE_WIDTH;
	m_collisionBox.h = m_yLightingRange * 2 * TILE_HEIGHT;
}

void ecc::SwitchableWindow::Render(SDL_Renderer* renderer, float speedFactor)
{
}

void ecc::SwitchableWindow::Render(SDL_Renderer* renderer, ObjectFactory* factory)
{
	factory->CreateWindow(renderer, m_lit ? m_onType : m_offType, m_xPos, m_yPos);
}

void ecc::SwitchableWindow::Switch(int group)
{
	if (!m_enable) return;

	if (m_group == group)
		m_lit = !m_lit;
}

void ecc::SwitchableWindow::SetEnable(bool status)
{
	m_enable = status;
}

const SDL_Rect& ecc::SwitchableWindow::GetCurrentDestination()
{
	return SDL_Rect();
}

const SDL_Rect& ecc::SwitchableWindow::GetCurrentDestination(ObjectFactory* factory)
{
	int width = 0;
	int height = 0;
	factory->GetImageDimension(m_onType, width, height);
	return { m_xPos, m_yPos, width, height };
}

bool ecc::SwitchableWindow::IsLit() const noexcept
{
	return m_lit;
}

void ecc::SwitchableWindow::GetLightingRange(int& x, int& y)
{
	x = m_xLightingRange;
	y = m_yLightingRange;
}
