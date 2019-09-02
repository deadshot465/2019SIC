#include "Camera.h"
#include "Helper.h"

ecc::Camera::Camera(SDL_Surface* windowSurface) : m_surfaceHandle(windowSurface)
{
	m_collisionBox.w = 384;
	m_collisionBox.h = 128;
}

ecc::Camera::~Camera()
{
}

void ecc::Camera::Move()
{
}

void ecc::Camera::SetCollisionBox()
{
}

void ecc::Camera::SetCollisionBox(int xLocation, int yLocation, int maxX, int maxY)
{
	SDL_DisplayMode display_mode = {};
	SDL_GetCurrentDisplayMode(0, &display_mode);

	m_collisionBox.x = xLocation;
	m_collisionBox.y = yLocation;
}