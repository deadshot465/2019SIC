#include "ICollidable.h"

bool ecc::ICollidable::CheckCollision(const SDL_Rect& other)
{
	/*int top_a = m_collisionBox.y;
	int bottom_a = top_a + m_collisionBox.h;
	int left_a = m_collisionBox.x;
	int right_a = left_a + m_collisionBox.w;

	int top_b = other.y;
	int bottom_b = top_b + other.h;
	int left_b = other.x;
	int right_b = left_b + other.w;

	if (bottom_a <= top_b) return false;
	if (top_a >= bottom_b) return false;
	if (right_a <= left_b) return false;
	if (left_a >= right_b) return false;

	return true;*/

	return SDL_HasIntersection(&m_collisionBox, &other);
}

const SDL_Rect& ecc::ICollidable::GetCollisionBox() const noexcept
{
	return m_collisionBox;
}