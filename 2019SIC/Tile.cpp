#include "Tile.h"
#include <algorithm>

ecc::Tile::Tile(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff, const SDL_Rect& destLocation, ecc::TileType tileType, bool isLit, bool isClimbable) :
	m_texCoordOn(texCoordOn), m_texCoordOff(texCoordOff), m_destLocation(destLocation), m_tileType(tileType), m_lit(isLit), m_isClimbable(isClimbable)
{
}

ecc::Tile::~Tile()
{
}

bool ecc::Tile::IsLit() const noexcept
{
	return m_lit;
}

void ecc::Tile::ChangeStatus(bool isLit) noexcept
{
	m_lit = isLit;
}

void ecc::Tile::SetClimbable(bool status) noexcept
{
	m_isClimbable = status;
}

const SDL_Rect& ecc::Tile::GetCurrentRect() const noexcept
{
	return m_lit ? m_texCoordOn : m_texCoordOff;
}

const SDL_Rect& ecc::Tile::GetDestinationLocation() const noexcept
{
	return m_destLocation;
}