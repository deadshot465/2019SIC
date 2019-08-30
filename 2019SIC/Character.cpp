#include "Character.h"
#include <algorithm>
#include <iostream>
#include "Helper.h"

ecc::Character::Character(SDL_Renderer* renderer,
	const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	const std::string& attackAnimationFileName,
	CharacterFlag characterFlag,
	int xPos, int yPos, float speed, ImageIndexFlag initialStatus) : m_currentImageIndex(initialStatus), m_characterFlag(characterFlag),
	m_startTime(std::chrono::high_resolution_clock::now())
{
	m_images.resize(MAX_ANIMATION_COUNT);
	m_frameCounts.resize(MAX_ANIMATION_COUNT);

	m_images[0] = std::make_unique<Image>();
	m_images[0]->LoadImage(renderer, waitAnimationFileName, true, xPos, yPos, false, 
		ImageIndexFlag::Idle);
	m_images[1] = std::make_unique<Image>();
	m_images[1]->LoadImage(renderer, moveAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Moving);
	m_images[2] = std::make_unique<Image>();
	m_images[2]->LoadImage(renderer, attackAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Attacking);
	
	m_speed = speed;

	m_frameCounts[0] = m_images[0]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[1] = m_images[1]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[2] = m_images[2]->m_width / CHARACTER_ATTACK_SPRITE_WIDTH;

	SetCollisionBox();
}

ecc::Character::~Character()
{
	IRenderable::~IRenderable();
}

void ecc::Character::Move()
{
	auto current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_D]) {

		m_flipMode = SDL_FLIP_NONE;

		for (auto& image : m_images) {
			image->MoveDestinationLocation(m_speed, 0);
			if (image->m_destinationLocation.x > RIGHT_X_BOUND)
				image->MoveDestinationLocation(-m_speed, 0);
		}

	}
	if (current_key_states[SDL_SCANCODE_A]) {

		m_flipMode = SDL_FLIP_HORIZONTAL;

		for (auto& image : m_images) {
			image->MoveDestinationLocation(-m_speed, 0);
			if (image->m_destinationLocation.x < LEFT_X_BOUND)
				image->MoveDestinationLocation(m_speed, 0);
		}

	}
	if (current_key_states[SDL_SCANCODE_W]) {
		//m_image->SetDestinationLocation(0, -m_speed);
	}
	if (current_key_states[SDL_SCANCODE_S]) {
		//m_image->SetDestinationLocation(0, m_speed);
	}

	if (current_key_states[SDL_SCANCODE_Z] &&
		m_currentImageIndex == ImageIndexFlag::Moving &&
		m_characterFlag == CharacterFlag::Father) {
		m_currentImageIndex = ImageIndexFlag::Attacking;
	}

	SetCollisionBox();
	SDL_Delay(50);
}

void ecc::Character::Render(SDL_Renderer* renderer, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode);
}

void ecc::Character::SetStatus(ImageIndexFlag status)
{
	m_currentImageIndex = status;
}

void ecc::Character::Animate(float speedFactor)
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	auto frame_duration =
		(static_cast<float>(m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) / 60) * speedFactor;

	auto elapsed_time =
		std::chrono::duration<float,
		std::chrono::seconds::period>(m_currentTime - m_startTime).count();

	if (elapsed_time > frame_duration) {

		switch (m_currentImageIndex)
		{
		case ecc::ImageIndexFlag::Idle:
		case ecc::ImageIndexFlag::Moving:
			m_currentXPos = m_currentFrame++ * CHARACTER_SPRITE_WIDTH;
			break;
		case ecc::ImageIndexFlag::Attacking:
			m_currentXPos = m_currentFrame++ * CHARACTER_ATTACK_SPRITE_WIDTH;
			break;
		default:
			break;
		}

		if (m_currentFrame >= m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) {
			m_currentFrame = 0;
		}

		m_startTime = m_currentTime;
	}

	m_images[static_cast<size_t>(m_currentImageIndex)]
		->SetTexCoord(m_currentXPos, m_images[static_cast<size_t>(m_currentImageIndex)]->m_texCoord.y);
	
}

void ecc::Character::SetCollisionBox()
{
	m_collisionBox.w = CHARACTER_SPRITE_WIDTH / 2;
	m_collisionBox.h = CHARACTER_SPRITE_HEIGHT;
	m_collisionBox.x =
		m_images[static_cast<size_t>(m_currentImageIndex)]
		->m_destinationLocation.x + (CHARACTER_SPRITE_WIDTH / 4);
	m_collisionBox.y =
		m_images[static_cast<size_t>(m_currentImageIndex)]->m_destinationLocation.y;
}

const SDL_Rect& ecc::Character::GetCurrentDestination()
{
	return m_images[0]->m_destinationLocation;
}

ecc::ImageIndexFlag ecc::Character::GetCurrentStatus() const noexcept
{
	return m_currentImageIndex;
}
