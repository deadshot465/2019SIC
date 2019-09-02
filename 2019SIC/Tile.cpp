#include "Tile.h"
#include <algorithm>

ecc::Tile::Tile(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff, const SDL_Rect& destLocation, ecc::TileType tileType, bool isLit) :
	m_texCoordOn(texCoordOn), m_texCoordOff(texCoordOff), m_destLocation(destLocation), m_tileType(tileType), m_lit(isLit)
{
}

ecc::Tile::~Tile()
{
}

bool ecc::Tile::IsLit() const noexcept
{
	return m_lit;
}

void ecc::Tile::ChangeStatus(bool isLit)
{
	m_lit = isLit;
}

const SDL_Rect& ecc::Tile::GetCurrentRect() const noexcept
{
	return m_lit ? m_texCoordOn : m_texCoordOff;
}

const SDL_Rect& ecc::Tile::GetDestinationLocation() const noexcept
{
	return m_destLocation;
}