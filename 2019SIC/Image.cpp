#include "Image.h"
#include <SDL_image.h>

void ecc::Image::LoadImage(SDL_Renderer* renderer, const std::string& fileName, bool transparency, bool isTile)
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

	SDL_FreeSurface(m_image);
}

void ecc::Image::LoadObject(SDL_Renderer* renderer, const std::string& fileName)
{
	LoadImage(renderer, fileName, true, false);

	m_texCoord = { 0, 0, m_width, m_height };
	m_destinationLocation = { 0, 0, m_width * 2, m_height * 2 };

}

void ecc::Image::LoadAnimation(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, int texWidth, int texHeight)
{
	LoadImage(renderer, fileName, true, false);

	m_texCoord = { 0, 0, texWidth, texHeight };
	m_destinationLocation = { xPos, yPos, texWidth * 2, texHeight * 2 };

}

void ecc::Image::LoadSprite(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier)
{
	LoadImage(renderer, fileName, transparency, false);

	m_texCoord = { 0, 0, m_width, m_height };
	float width = static_cast<float>(m_width) * magnifier;
	float height = static_cast<float>(m_height) * magnifier;
	m_destinationLocation = { xPos, yPos, static_cast<int>(width), static_cast<int>(height) };
}

void ecc::Image::LoadAnimatedSprite(SDL_Renderer* renderer, const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier, int clipX, int clipY)
{
	LoadImage(renderer, fileName, transparency, false);

	m_texCoord = { 0, 0, clipX, clipY };
	float width = clipX * magnifier;
	float height = clipY * magnifier;
	m_destinationLocation = { xPos, yPos, static_cast<int>(width), static_cast<int>(height) };
}

void ecc::Image::LoadCharacterSprites(SDL_Renderer* renderer, const std::string& fileName,
	ecc::ImageIndexFlag imageFlag, int xPos, int yPos)
{
	LoadImage(renderer, fileName, true, false);

	if (imageFlag == ImageIndexFlag::Attack) {
		m_texCoord = { 0, 0, CHARACTER_ATTACK_SPRITE_WIDTH, CHARACTER_ATTACK_SPRITE_HEIGHT };
		m_destinationLocation = { xPos, yPos, CHARACTER_ATTACK_SPRITE_WIDTH * 2, CHARACTER_ATTACK_SPRITE_HEIGHT * 2 };
	}
	else {
		m_texCoord = { 0, 0, CHARACTER_SPRITE_WIDTH, CHARACTER_SPRITE_HEIGHT };
		m_destinationLocation = { xPos, yPos, CHARACTER_SPRITE_WIDTH * 2, CHARACTER_SPRITE_HEIGHT * 2 };
	}
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
	m_destinationLocation.x += static_cast<int>(x);
	m_destinationLocation.y += static_cast<int>(y);
}

void ecc::Image::SetTexCoord(int x, int y) noexcept
{
	m_texCoord.x = x;
	m_texCoord.y = y;
}

void ecc::Image::Render(SDL_Renderer* renderer, SDL_RendererFlip flipMode, double angle,
	const SDL_Point* center, int offsetX, int offsetY)
{
	auto dst_rect = m_destinationLocation;
	dst_rect.x -= offsetX;
	dst_rect.y -= offsetY;
	SDL_RenderCopyEx(renderer, m_texture, &m_texCoord, &dst_rect, angle, center, flipMode);
}

void ecc::Image::RenderObject(SDL_Renderer* renderer, int xPos, int yPos)
{
	auto dst_rect = m_destinationLocation;
	dst_rect.x = xPos;
	dst_rect.y = yPos;
	SDL_RenderCopy(renderer, m_texture, &m_texCoord, &dst_rect);
}
