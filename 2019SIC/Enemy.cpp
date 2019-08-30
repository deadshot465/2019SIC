#include "Enemy.h"
#include <iostream>
#include "Helper.h"

ecc::Enemy::Enemy(SDL_Renderer* renderer, const std::string& waitAnimationFileName, const std::string& moveAnimationFileName, int xPos, int yPos, float speed)
{
	m_image = std::make_unique<Image>();
	m_image->LoadImage(renderer, waitAnimationFileName, true, xPos, yPos);

	m_speed = speed;
	
	m_enemyCurrentSpeed = m_enemyInitialSpeed;

	SetCollisionBox();
}

ecc::Enemy::~Enemy()
{
	IRenderable::~IRenderable();
}

void ecc::Enemy::Move()
{
	
}

void ecc::Enemy::Move(int nextDestination)
{
	static int move_direction = 0;

	if (!m_moveFinished) {

		m_nextDestination = nextDestination;

		int direction = (m_nextDestination * TILE_WIDTH) - m_image->m_destinationLocation.x;
		m_flipMode = (direction > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		move_direction = (direction > 0) ? m_enemyCurrentSpeed : -m_enemyCurrentSpeed;

		if (direction > 0) {
			m_image->MoveDestinationLocation(move_direction, 0);

			if (m_image->m_destinationLocation.x > RIGHT_X_BOUND) {
				m_image->MoveDestinationLocation(-move_direction, 0);
				m_moveFinished = true;
				return;
			}
		}
		else {
			m_image->MoveDestinationLocation(move_direction, 0);

			if (m_image->m_destinationLocation.x < LEFT_X_BOUND) {
				m_image->MoveDestinationLocation(-move_direction, 0);
				m_moveFinished = true;
				return;
			}

		}
	}
	
}

void ecc::Enemy::Attack()
{

}

void ecc::Enemy::SetCollisionBox()
{
	m_collisionBox.w = CHARACTER_SPRITE_WIDTH / 2;
	m_collisionBox.h = CHARACTER_SPRITE_HEIGHT;
	m_collisionBox.x = m_image->m_destinationLocation.x + (CHARACTER_SPRITE_WIDTH / 4);
	m_collisionBox.y = m_image->m_destinationLocation.y;
}

void ecc::Enemy::Render(SDL_Renderer* renderer, float speedFactor)
{
	m_image->Render(renderer, m_flipMode);
}

void ecc::Enemy::Animate(float speedFactor)
{
}

const SDL_Rect& ecc::Enemy::GetCurrentDestination()
{
	return m_image->m_destinationLocation;
}

void ecc::Enemy::StartMove() noexcept
{
	m_moveFinished = false;
}

bool ecc::Enemy::IsMoveFinished() const noexcept
{
	return m_moveFinished;
}
