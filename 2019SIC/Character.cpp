#include "Character.h"
#include <algorithm>
#include <iostream>
#include "Helper.h"

ecc::Character::Character(SDL_Renderer* renderer,
	const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	const std::string& attackAnimationFileName,
	const std::string& jumpAnimationFileName,
	const std::string& fallAnimationFileName,
	CharacterFlag characterFlag,
	int xPos, int yPos, float speed,
	float idleSpeed, float moveSpeed, float attackSpeed, float jumpSpeed, float fallSpeed,
	ImageIndexFlag initialStatus) :
	m_currentImageIndex(initialStatus), m_characterFlag(characterFlag)
{
	m_images.resize(MAX_ANIMATION_COUNT);
	m_frameCounts.resize(MAX_ANIMATION_COUNT);
	m_animationSpeeds.resize(MAX_ANIMATION_COUNT);

	m_images[0] = std::make_unique<Image>();
	m_images[0]->LoadImage(renderer, waitAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Idle);
	m_images[1] = std::make_unique<Image>();
	m_images[1]->LoadImage(renderer, moveAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Move);
	m_images[2] = std::make_unique<Image>();
	m_images[2]->LoadImage(renderer, attackAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Attack);
	m_images[3] = std::make_unique<Image>();
	m_images[3]->LoadImage(renderer, jumpAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Jump);
	m_images[4] = std::make_unique<Image>();
	m_images[4]->LoadImage(renderer, fallAnimationFileName, true, xPos, yPos, false,
		ImageIndexFlag::Fall);

	m_frameCounts[0] = m_images[0]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[1] = m_images[1]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[2] = m_images[2]->m_width / CHARACTER_ATTACK_SPRITE_WIDTH;
	m_frameCounts[3] = m_images[3]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[4] = m_images[4]->m_width / CHARACTER_SPRITE_WIDTH;

	m_animationSpeeds[0] = idleSpeed;
	m_animationSpeeds[1] = moveSpeed;
	m_animationSpeeds[2] = attackSpeed;
	m_animationSpeeds[3] = jumpSpeed;
	m_animationSpeeds[4] = fallSpeed;

	m_speed = speed;
	m_startTime = std::chrono::high_resolution_clock::now();

	if (characterFlag == CharacterFlag::Father) {
		m_statusFlag = CharacterStatusFlag::Controllable;
	}
	else {
		m_statusFlag = CharacterStatusFlag::Incontrollable;
	}

	SetCollisionBox();
}

ecc::Character::~Character()
{
	IRenderable::~IRenderable();
}

void ecc::Character::Move()
{
}

void ecc::Character::Move(SDL_Surface* windowSurface)
{
	if (m_statusFlag == CharacterStatusFlag::Incontrollable) return;

	auto current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_D]) {
		m_flipMode = SDL_FLIP_NONE;

		if (!m_animationStarted && !m_jumpStarted) {
			m_currentImageIndex = ImageIndexFlag::Move;
			m_animationStarted = true;
			m_animationTimer = 0;
		}

		for (auto& image : m_images) {
			image->MoveDestinationLocation(m_speed, 0);
			if (image->m_destinationLocation.x + CHARACTER_SPRITE_WIDTH >
				(windowSurface->w - (TILE_WIDTH * 3)))
				image->MoveDestinationLocation(-m_speed, 0);
		}
	}

	if (current_key_states[SDL_SCANCODE_A]) {
		m_flipMode = SDL_FLIP_HORIZONTAL;

		if (!m_animationStarted && !m_jumpStarted) {
			m_currentImageIndex = ImageIndexFlag::Move;
			m_animationStarted = true;
			m_animationTimer = 0;
		}

		for (auto& image : m_images) {
			image->MoveDestinationLocation(-m_speed, 0);
			if (image->m_destinationLocation.x < 0)
				image->MoveDestinationLocation(m_speed, 0);
		}
	}

	if (current_key_states[SDL_SCANCODE_Z] &&
		m_characterFlag == CharacterFlag::Father) {
		m_currentImageIndex = ImageIndexFlag::Attack;
	}

	SetCollisionBox();
	SDL_Delay(50);
}

void ecc::Character::Render(SDL_Renderer* renderer, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode);
}

void ecc::Character::Render(SDL_Renderer* renderer, int offsetX, int offsetY, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode, 0.0,
		nullptr, offsetX, offsetY);
}

void ecc::Character::SetStatus(ecc::CharacterStatusFlag status)
{
	m_statusFlag = status;
}

void ecc::Character::Animate(float speedFactor)
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	auto frame_duration =
		(static_cast<float>(m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) / 60) * m_animationSpeeds[static_cast<size_t>(m_currentImageIndex)];

	auto elapsed_time =
		std::chrono::duration<float,
		std::chrono::seconds::period>(m_currentTime - m_startTime).count();

	if ((m_animationStarted || m_jumpStarted) && m_currentFrame >= m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) {
		m_currentFrame = 0;
	}

	if (elapsed_time > frame_duration) {
		switch (m_currentImageIndex)
		{
		case ecc::ImageIndexFlag::Idle:
		case ecc::ImageIndexFlag::Move:
		case ecc::ImageIndexFlag::Jump:
		case ecc::ImageIndexFlag::Fall:
			m_currentXPos = m_currentFrame++ * CHARACTER_SPRITE_WIDTH;
			break;
		case ecc::ImageIndexFlag::Attack:
			m_currentXPos = m_currentFrame++ * CHARACTER_ATTACK_SPRITE_WIDTH;
			break;
		default:
			break;
		}

		if (m_animationStarted) ++m_animationTimer;

		if (m_currentFrame >= m_frameCounts[static_cast<size_t>(m_currentImageIndex)]) {
			if (m_animationStarted) {
				m_animationTimer = 0;
				m_animationStarted = false;
				auto key_state = SDL_GetKeyboardState(nullptr);
				if (!key_state[SDL_SCANCODE_D] &&
					!key_state[SDL_SCANCODE_A])
					m_currentImageIndex = ImageIndexFlag::Idle;
			}

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

void ecc::Character::Jump()
{
	auto key_state = SDL_GetKeyboardState(nullptr);

	if (key_state[SDL_SCANCODE_W] && !m_jumpStarted) {
		m_jumpStartedYCoordinate = m_images[0]->m_destinationLocation.y;
		m_jumpStarted = true;
		m_jumpTimer = 10;
		m_currentImageIndex = ImageIndexFlag::Jump;
	}

	if (m_jumpStarted) {
		if (m_jumpTimer <= 0) {
			m_currentImageIndex = ImageIndexFlag::Fall;
			m_fallStarted = true;
		}

		for (auto& image : m_images) {
			image->MoveDestinationLocation(0, m_fallStarted ? m_jumpSpeed : -m_jumpSpeed);
		}
		m_jumpTimer -= 2;

		if (m_fallStarted && m_images[0]->m_destinationLocation.y >= m_jumpStartedYCoordinate) {
			m_jumpStarted = false;
			m_fallStarted = false;
			m_currentImageIndex = ImageIndexFlag::Idle;
		}
	}
}