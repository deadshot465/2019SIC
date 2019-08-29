#include "Character.h"
#include <chrono>
#include <iostream>
#include "Helper.h"

ecc::Character::Character(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, float speed)
{
	m_image = std::make_unique<Image>();
	m_image->LoadImage(renderer, fileName, true, xPos, yPos);
	m_speed = speed;

	m_frameCount = m_image->m_width / CHARACTER_SPRITE_WIDTH;
}

ecc::Character::~Character()
{
	m_image.reset();
}

void ecc::Character::Move()
{
	auto current_key_states = SDL_GetKeyboardState(nullptr);

	if (current_key_states[SDL_SCANCODE_D]) {
		m_image->MoveDestinationLocation(m_speed, 0);
	}
	if (current_key_states[SDL_SCANCODE_A]) {
		m_image->MoveDestinationLocation(-m_speed, 0);
	}
	if (current_key_states[SDL_SCANCODE_W]) {
		//m_image->SetDestinationLocation(0, -m_speed);
	}
	if (current_key_states[SDL_SCANCODE_S]) {
		//m_image->SetDestinationLocation(0, m_speed);
	}

	SDL_Delay(50);
}

void ecc::Character::Render(SDL_Renderer* renderer)
{
	Animate();
	SDL_RenderCopy(renderer, m_image->GetTexture(), &(m_image->GetTexCoord()), &(m_image->GetDestinationLocation()));
}

void ecc::Character::Animate()
{
	static auto current_frame = 0;
	static auto start_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

	float speed = m_frameCount / 60;

	auto elapsed_time =
		std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

	int x = 0;
	if (elapsed_time > speed) {

		x = current_frame++ * CHARACTER_SPRITE_WIDTH;
		
		if (current_frame == m_frameCount) {
			current_frame = 0;
		}

	}

	m_image->SetTexCoord(x, m_image->GetTexCoord().y);
}
