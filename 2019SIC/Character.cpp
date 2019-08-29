#include "Character.h"
#include <chrono>
#include <iostream>
#include "Helper.h"

ecc::Character::Character(SDL_Renderer* renderer,
	const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	int xPos, int yPos, float speed)
{
	m_images.resize(MAX_ANIMATION_COUNT);
	m_frameCounts.resize(MAX_ANIMATION_COUNT);

	m_images[0] = std::make_unique<Image>();
	m_images[0]->LoadImage(renderer, waitAnimationFileName, true, xPos, yPos);
	m_images[1] = std::make_unique<Image>();
	m_images[1]->LoadImage(renderer, moveAnimationFileName, true, xPos, yPos);
	
	m_speed = speed;

	m_frameCounts[0] = m_images[0]->m_width / CHARACTER_SPRITE_WIDTH;
	m_frameCounts[1] = m_images[1]->m_width / CHARACTER_SPRITE_WIDTH;

	SetCollisionBox();
}

ecc::Character::~Character()
{
	for (auto& image : m_images) {
		image.reset();
	}
}

void ecc::Character::Move()
{
	auto current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_D]) {

		for (auto& image : m_images) {
			image->MoveDestinationLocation(m_speed, 0);
			if (image->m_destinationLocation.x > RIGHT_X_BOUND)
				image->MoveDestinationLocation(-m_speed, 0);
		}

	}
	if (current_key_states[SDL_SCANCODE_A]) {

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

	SetCollisionBox();
}

void ecc::Character::Render(SDL_Renderer* renderer)
{
	Animate();
	m_images[m_currentImageIndex]->Render(renderer);
	SDL_Delay(50);
}

void ecc::Character::SetStatus(unsigned short status)
{
	m_currentImageIndex = status;
}

void ecc::Character::Animate()
{
	static unsigned int current_idle_frame = 0;
	static unsigned int current_moving_frame = 0;

	static auto start_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

	auto speed = static_cast<float>(m_frameCounts[m_currentImageIndex]) / 60;

	auto elapsed_time =
		std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

	int x = 0;
	if (elapsed_time > speed) {

		if (m_currentImageIndex == 0) {
			x = current_idle_frame++ * CHARACTER_SPRITE_WIDTH;

			if (current_idle_frame >= m_frameCounts[m_currentImageIndex]) {
				current_idle_frame = 0;
			}
		}
		else {
			x = current_moving_frame++ * CHARACTER_SPRITE_WIDTH;

			if (current_moving_frame >= m_frameCounts[m_currentImageIndex]) {
				current_moving_frame = 0;
			}
		}

	}

	m_images[m_currentImageIndex]->SetTexCoord(x, m_images[m_currentImageIndex]->m_texCoord.y);
}

void ecc::Character::SetCollisionBox()
{
	m_collisionBox.w = CHARACTER_SPRITE_WIDTH / 2;
	m_collisionBox.h = CHARACTER_SPRITE_HEIGHT;
	m_collisionBox.x = m_images[m_currentImageIndex]->m_destinationLocation.x + (CHARACTER_SPRITE_WIDTH / 4);
	m_collisionBox.y = m_images[m_currentImageIndex]->m_destinationLocation.y;
}

const SDL_Rect& ecc::Character::GetCurrentDestination() const noexcept
{
	return m_images[0]->m_destinationLocation;
}
