#include "Object.h"

ecc::Object::Object(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos)
{
	m_image = std::make_unique<Image>();
	m_image->LoadImage(renderer, fileName, true, xPos, yPos);

	SetCollisionBox();
}

ecc::Object::~Object()
{
	IRenderable::~IRenderable();
}

void ecc::Object::SetCollisionBox()
{
	m_collisionBox.w = m_image->m_width;
	m_collisionBox.h = m_image->m_height;
	m_collisionBox.x = m_image->m_destinationLocation.x;
	m_collisionBox.y = m_image->m_destinationLocation.y;
}

void ecc::Object::Render(SDL_Renderer* renderer, float speedFactor)
{
	m_image->Render(renderer);
}

const SDL_Rect& ecc::Object::GetCurrentDestination()
{
	return m_image->m_destinationLocation;
}