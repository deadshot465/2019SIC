#include "Character.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include "Helper.h"

ecc::Character::Character(SDL_Renderer* renderer,
	const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	const std::string& attackAnimationFileName,
	const std::string& jumpAnimationFileName,
	const std::string& fallAnimationFileName,
	const std::string& climbAnimationFileName,
	CharacterFlag characterFlag,
	int xPos, int yPos, float speed,
	float idleSpeed, float moveSpeed, float attackSpeed, float jumpSpeed, float fallSpeed,
	float climbSpeed,
	ImageIndexFlag initialStatus) :
	m_currentImageIndex(initialStatus), m_characterFlag(characterFlag)
{
	m_images.resize(MAX_ANIMATION_COUNT);
	m_frameCounts.resize(MAX_ANIMATION_COUNT);
	m_animationSpeeds.resize(MAX_ANIMATION_COUNT);

	m_images[0] = std::make_unique<Image>();
	m_images[0]->LoadCharacterSprites(renderer, waitAnimationFileName, ImageIndexFlag::Idle, xPos, yPos);
	
	m_images[1] = std::make_unique<Image>();
	m_images[1]->LoadCharacterSprites(renderer, moveAnimationFileName, ImageIndexFlag::Move, xPos, yPos);

	m_images[2] = std::make_unique<Image>();
	m_images[2]->LoadCharacterSprites(renderer, attackAnimationFileName, ImageIndexFlag::Attack, xPos, yPos);

	m_images[3] = std::make_unique<Image>();
	m_images[3]->LoadCharacterSprites(renderer, jumpAnimationFileName, ImageIndexFlag::Jump, xPos, yPos);

	m_images[4] = std::make_unique<Image>();
	m_images[4]->LoadCharacterSprites(renderer, fallAnimationFileName, ImageIndexFlag::Fall, xPos, yPos);

	m_images[5] = std::make_unique<Image>();
	m_images[5]->LoadCharacterSprites(renderer, climbAnimationFileName, ImageIndexFlag::Climb, xPos, yPos);

	m_frameCounts[0] = m_images[0]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[1] = m_images[1]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[2] = m_images[2]->m_width / CHARACTER_ATTACK_SPRITE_WIDTH;
	m_frameCounts[3] = m_images[3]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[4] = m_images[4]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[5] = m_images[5]->m_width / CHARACTER_SPRITE_WIDTH;

	m_animationSpeeds[0] = idleSpeed;
	m_animationSpeeds[1] = moveSpeed;
	m_animationSpeeds[2] = attackSpeed;
	m_animationSpeeds[3] = jumpSpeed;
	m_animationSpeeds[4] = fallSpeed;
	m_animationSpeeds[5] = climbSpeed;

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
	assert(m_statusFlag != CharacterStatusFlag::Controllable);

	static const auto original_x = static_cast<double>(m_images[0]->m_destinationLocation.x) / CHARACTER_SPRITE_WIDTH;

	static auto next_destination = 0;
	static auto idle_flag = false;
	
	static const auto timer_limit = 5.0;
	static auto timer_started = false;
	static auto timer_start_time = std::chrono::time_point<std::chrono::steady_clock>();
	static auto timer = 0.0;

	if (!m_animationStarted && !idle_flag) {
		while (next_destination == 0)
			next_destination = GetRandomInt(-12, 12);
		m_flipMode = (next_destination > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		m_currentImageIndex = ImageIndexFlag::Move;
		m_animationStarted = true;
	}
	else if (m_animationStarted && !idle_flag) {
		if (next_destination > 0) {
			for (auto& image : m_images) {
				image->MoveDestinationLocation(m_speed, 0);
			}
		}
		else {
			for (auto& image : m_images) {
				image->MoveDestinationLocation(-m_speed, 0);
			}
		}
		next_destination += (next_destination > 0) ? -1 : 1;

		if (next_destination == 0) {
			m_currentImageIndex = ImageIndexFlag::Idle;
			m_animationStarted = false;
			idle_flag = true;
		}
	}
	else if (!m_animationStarted && idle_flag) {
		if (!timer_started) {
			timer = GetRandomDouble(0.0, timer_limit);
			timer_start_time = std::chrono::high_resolution_clock::now();
			timer_started = true;
		}
		else {
			auto current_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration<double, std::chrono::seconds::period>(current_time - timer_start_time).count();

			if (duration > timer) {
				idle_flag = false;
				timer_started = false;

				auto current_offset = static_cast<double>(m_images[0]->m_destinationLocation.x) / CHARACTER_SPRITE_WIDTH;
				if (std::abs(current_offset - original_x) >= 18) {
					if (current_offset < original_x) {
						next_destination = static_cast<int>(std::abs(current_offset - original_x));
					}
					else {
						next_destination = static_cast<int>(std::abs(current_offset - original_x)) * -1;
					}
				}
			}
		}
	}
}

void ecc::Character::Move(ecc::Mixer* mixerHandle, SDL_Surface* windowSurface)
{
	assert(m_statusFlag != CharacterStatusFlag::Incontrollable);

	auto current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_RIGHT] && !m_climbUpStarted && !m_climbDownStarted) {
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

	if (current_key_states[SDL_SCANCODE_LEFT] && !m_climbUpStarted && !m_climbDownStarted) {
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
		m_characterFlag == CharacterFlag::Father &&
		!m_climbUpStarted && !m_climbDownStarted) {

		if (!m_animationStarted && !m_jumpStarted) {
			m_currentImageIndex = ImageIndexFlag::Attack;
			auto music_number = GetRandomInt(0, 2);
			switch (music_number) {
			case 0:
				mixerHandle->PlaySound(SoundList::Punch1, 1);
				break;
			case 1:
				mixerHandle->PlaySound(SoundList::Punch2, 1);
				break;
			case 2:
				mixerHandle->PlaySound(SoundList::Punch3, 1);
				break;
			}
			m_animationStarted = true;
			m_animationTimer = 0;
		}

	}

	SetCollisionBox();
	SDL_Delay(50);
}

void ecc::Character::Render(SDL_Renderer* renderer, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode);
	SetCollisionBox();
	SetAttackCollisionBox();
}

void ecc::Character::Render(SDL_Renderer* renderer, int offsetX, int offsetY, float speedFactor)
{
	Animate(speedFactor);
	m_images[static_cast<size_t>(m_currentImageIndex)]->Render(renderer, m_flipMode, 0.0,
		nullptr, offsetX, offsetY);
	SetCollisionBox();
	SetAttackCollisionBox();
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
		case ecc::ImageIndexFlag::Climb:
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
			if (m_animationStarted && m_statusFlag == CharacterStatusFlag::Controllable) {
				m_animationTimer = 0;
				m_animationStarted = false;
				auto key_state = SDL_GetKeyboardState(nullptr);
				if (!key_state[SDL_SCANCODE_RIGHT] &&
					!key_state[SDL_SCANCODE_LEFT])
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
	m_collisionBox.w = 48;
	m_collisionBox.h = 96;
	m_collisionBox.x =
		m_images[static_cast<size_t>(m_currentImageIndex)]
		->m_destinationLocation.x + 24;
	m_collisionBox.y =
		m_images[static_cast<size_t>(m_currentImageIndex)]
		->m_destinationLocation.y + 32;
}

void ecc::Character::SetAttackCollisionBox()
{
	m_attackCollisionBox.w = 48;
	m_attackCollisionBox.h = 96;
	m_attackCollisionBox.x = m_images[static_cast<size_t>(m_currentImageIndex)]
		->m_destinationLocation.x + ((m_flipMode == SDL_FLIP_NONE) ? 88 : -12);
	m_attackCollisionBox.y =
		m_images[static_cast<size_t>(m_currentImageIndex)]
		->m_destinationLocation.y + 32;
}

const SDL_Rect& ecc::Character::GetAttackCollisionBox() const noexcept
{
	return m_attackCollisionBox;
}

bool ecc::Character::CheckAttackCollision(const SDL_Rect& other) const noexcept
{
	auto key_state = SDL_GetKeyboardState(nullptr);

	return SDL_HasIntersection(&m_attackCollisionBox, &other) && key_state[SDL_SCANCODE_Z];
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

	if (key_state[SDL_SCANCODE_SPACE] && !m_jumpStarted) {
		m_jumpStartedYCoordinate = m_images[0]->m_destinationLocation.y;
		m_jumpStarted = true;
		m_jumpTimer = (m_characterFlag == CharacterFlag::Father) ? 22 : 10;
		m_currentImageIndex = ImageIndexFlag::Jump;
	}

	if (m_jumpStarted) {
		if (m_jumpTimer <= 0 && !m_fallStarted) {
			m_currentImageIndex = ImageIndexFlag::Fall;
			m_fallStarted = true;
		}

		for (auto& image : m_images) {
			image->MoveDestinationLocation(0, m_fallStarted ? static_cast<float>(-m_jumpTimer) : static_cast<float>(-m_jumpTimer));
		}
		m_jumpTimer -= 2;

		if (m_fallStarted && m_images[0]->m_destinationLocation.y >= m_jumpStartedYCoordinate) {
			m_jumpStarted = false;
			m_fallStarted = false;
			m_currentImageIndex = ImageIndexFlag::Idle;
		}
	}
}

void ecc::Character::Climb(int xPos, int yPos)
{
	for (const auto& image : m_images) {
		image->m_destinationLocation.x = xPos;
		image->m_destinationLocation.y = yPos;
	}
}

void ecc::Character::Fall()
{
	for (const auto& image : m_images) {
		image->m_destinationLocation.y += 1;
	}
}
