#include "Object.h"

ecc::Object::Object(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos)
{
	m_image = std::make_unique<Image>();
	m_image->LoadImage(renderer, fileName, true, xPos, yPos);

	SetCollisionBox();
}

ecc::Object::~Object()
{
	m_image.reset();
}

void ecc::Object::SetCollisionBox()
{
	m_collisionBox.w = m_image->m_width;
	m_collisionBox.h = m_image->m_height;
	m_collisionBox.x = m_image->m_destinationLocation.x;
	m_collisionBox.y = m_image->m_destinationLocation.y;
}

void ecc::Object::Render(SDL_Renderer* renderer)
{
	m_image->Render(renderer);
}
