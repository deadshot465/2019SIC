#include "Image.h"
#include <SDL_image.h>
#include "Helper.h"

void ecc::Image::LoadImage(SDL_Renderer* renderer, const std::string& fileName,
	bool transparency, int xPos, int yPos, bool isTile)
{
	m_image = IMG_Load(fileName.c_str());

	if (transparency) {
		SDL_SetColorKey(m_image, SDL_TRUE, SDL_MapRGB(m_image->format,
			TRANSPARENT_R, TRANSPARENT_G, TRANSPARENT_B));
	}

	m_texture = SDL_CreateTextureFromSurface(renderer, m_image);
	m_width = m_image->w;
	m_height = m_image->h;
	m_isTile = isTile;

	m_texCoord = { 0, 0, CHARACTER_SPRITE_WIDTH, CHARACTER_SPRITE_HEIGHT };
	m_destinationLocation = { xPos, yPos, CHARACTER_SPRITE_WIDTH, CHARACTER_SPRITE_HEIGHT };

	SDL_FreeSurface(m_image);
}

ecc::Image::Image()
{
	
}

ecc::Image::~Image()
{
	SDL_DestroyTexture(m_texture);
}

void ecc::Image::MoveDestinationLocation(float x, float y) noexcept
{
	m_destinationLocation.x += x;
	m_destinationLocation.y += y;
}

void ecc::Image::SetTexCoord(int x, int y) noexcept
{
	m_texCoord.x = x;
	m_texCoord.y = y;
}

void ecc::Image::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, m_texture, &m_texCoord, &m_destinationLocation);
}
