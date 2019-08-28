#include "Image.h"
#include <SDL_image.h>

void ecc::Image::LoadImage(SDL_Renderer* renderer, const std::string& fileName)
{
	m_image = IMG_Load(fileName.c_str());
	m_texture = SDL_CreateTextureFromSurface(renderer, m_image);
	m_width = m_image->w;
	m_height = m_image->h;
}

ecc::Image::Image()
{
	
}

ecc::Image::~Image()
{
	SDL_DestroyTexture(m_texture);
	SDL_FreeSurface(m_image);
}

SDL_Texture* ecc::Image::GetTexture() const noexcept
{
	return m_texture;
}

SDL_Rect ecc::Image::GetRect() const noexcept
{
	auto rect = SDL_Rect();
	return rect;
}
