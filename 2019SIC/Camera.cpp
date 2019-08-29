#include "Camera.h"
#include "Helper.h"

void ecc::Camera::Move()
{
}

void ecc::Camera::SetCollisionBox()
{
	
}

void ecc::Camera::SetCollisionBox(int xLocation, int yLocation)
{
	SDL_DisplayMode display_mode = {};
	SDL_GetCurrentDisplayMode(0, &display_mode);

	m_collisionBox.w = MAP_WIDTH / 4;
	m_collisionBox.h = MAP_HEIGHT / 4;

	m_collisionBox.x = (xLocation + m_collisionBox.w) > MAP_WIDTH ? (MAP_WIDTH - m_collisionBox.w) : xLocation;
	m_collisionBox.y = (yLocation + m_collisionBox.h) > MAP_HEIGHT ? (MAP_HEIGHT - m_collisionBox.h) : yLocation;
}
