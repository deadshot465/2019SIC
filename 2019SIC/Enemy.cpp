#include "Enemy.h"
#include <iostream>
#include "Helper.h"

ecc::Enemy::Enemy(SDL_Renderer* renderer, const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	const std::string& attackAnimationFileName,
	int xPos, int yPos, float speed,
	double idleSpeed, double moveSpeed, double attackSpeed,
	int moveRange) :
	m_enemyRushSpeed(4 * speed), m_enemyMoveRange(moveRange), m_originalX(xPos)
{
	m_images.resize(3);
	m_frameCounts.resize(3);
	m_animationSpeeds.resize(3);

	m_images[0] = std::make_unique<Image>();
	m_images[0]->LoadImage(renderer, waitAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Idle);
	m_images[1] = std::make_unique<Image>();
	m_images[1]->LoadImage(renderer, moveAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Move);
	m_images[2] = std::make_unique<Image>();
	m_images[2]->LoadImage(renderer, attackAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Move);

	m_frameCounts[0] = m_images[0]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[1] = m_images[1]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[2] = m_images[2]->m_width / CHARACTER_SPRITE_WIDTH;

	m_animationSpeeds[0] = idleSpeed;
	m_animationSpeeds[1] = moveSpeed;
	m_animationSpeeds[2] = attackSpeed;

	m_speed = speed;
	m_startTime = std::chrono::high_resolution_clock::now();

	SetCollisionBox();
}

ecc::Enemy::~Enemy()
{
	IRenderable::~IRenderable();
}

void ecc::Enemy::Move()
{
}

void ecc::Enemy::Move(const SDL_Rect& playerPosition, bool foundPlayer, SDL_Surface* windowSurface)
{
	if (!foundPlayer) {
		if (m_moveFinished) {
			StartMove();
		}
		else {
			if (m_nextDestination > 0) {
				m_flipMode = SDL_FLIP_NONE;

				for (const auto& image : m_images) {
					image->MoveDestinationLocation(m_speed, 0);
					if (image->m_destinationLocation.x + CHARACTER_SPRITE_WIDTH >
						(windowSurface->w - (TILE_WIDTH * 3)))
						image->MoveDestinationLocation(-m_speed, 0);

					if (image->m_destinationLocation.x > m_targetDestination) {
						m_moveFinished = true;
						m_nextDestination = -1;
					}
				}
			}
			else {
				m_flipMode = SDL_FLIP_HORIZONTAL;

				for (const auto& image : m_images) {
					image->MoveDestinationLocation(-m_speed, 0);
					if (image->m_destinationLocation.x < 0)
						image->MoveDestinationLocation(m_speed, 0);

					if (image->m_destinationLocation.x < m_targetDestination) {
						m_moveFinished = true;
						m_nextDestination = 1;
					}
				}
			}

		}
	}
	else {
		if (m_moveFinished) {
			m_moveFinished = false;
			if (playerPosition.x < m_images[0]->m_destinationLocation.x) {
				m_flipMode = SDL_FLIP_HORIZONTAL;
			}
			else {
				m_flipMode = SDL_FLIP_NONE;
			}
			m_nextDestination = playerPosition.x;
			return;
		}
		else {
			switch (m_flipMode)
			{
			case SDL_FLIP_HORIZONTAL:
				for (const auto& image : m_images) {
					image->MoveDestinationLocation(-m_enemyRushSpeed, 0);
					if (image->m_destinationLocation.x <= m_nextDestination) {
						m_moveFinished = true;
						m_nextDestination = 1;
					}
				}
				break;
			case SDL_FLIP_NONE:
				for (const auto& image : m_images) {
					image->MoveDestinationLocation(m_enemyRushSpeed, 0);
					if (image->m_destinationLocation.x >= m_nextDestination) {
						m_moveFinished = true;
						m_nextDestination = -1;
					}
				}
				break;
			default:
				break;
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
	m_collisionBox.x = m_images[0]->m_destinationLocation.x + (CHARACTER_SPRITE_WIDTH / 4);
	m_collisionBox.y = m_images[0]->m_destinationLocation.y;
}

void ecc::Enemy::Render(SDL_Renderer* renderer, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode);
}

void ecc::Enemy::Animate(float speedFactor)
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	auto frame_duration =
		(static_cast<double>(m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) / 60) * m_animationSpeeds[static_cast<size_t>(m_currentImageIndex)];

	auto elapsed_time =
		std::chrono::duration<double,
		std::chrono::seconds::period>(m_currentTime - m_startTime).count();

	if (elapsed_time > frame_duration) {
		
		m_currentXPos = m_currentFrame++ * CHARACTER_SPRITE_WIDTH;

		if (m_currentFrame >= m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) {
			m_currentFrame = 0;
		}

		m_startTime = m_currentTime;
	}

	m_images[static_cast<size_t>(m_currentImageIndex)]
		->SetTexCoord(m_currentXPos, m_images[static_cast<size_t>(m_currentImageIndex)]->m_texCoord.y);
}

const SDL_Rect& ecc::Enemy::GetCurrentDestination()
{
	return m_images[0]->m_destinationLocation;
}

void ecc::Enemy::StartMove() noexcept
{
	m_moveFinished = false;
	m_targetDestination = m_originalX + (m_enemyMoveRange * m_nextDestination * (TILE_WIDTH * 2));
}

bool ecc::Enemy::IsMoveFinished() const noexcept
{
	return m_moveFinished;
}