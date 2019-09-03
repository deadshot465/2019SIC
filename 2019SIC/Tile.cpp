#include "Tile.h"
#include <algorithm>

ecc::Tile::Tile(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff, const SDL_Rect& destLocation, ecc::TileType tileType, bool isLit, bool isClimbable, bool isWalkable, bool isBlocked) :
	m_texCoordOn(texCoordOn), m_texCoordOff(texCoordOff), m_destLocation(destLocation), m_tileType(tileType), m_lit(isLit), m_isClimbable(isClimbable), m_isWalkable(isWalkable), m_isBlocked(isBlocked)
{
	SetCollisionBox();
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

void ecc::Tile::SetWalkable(bool status) noexcept
{
	m_isWalkable = status;
}

void ecc::Tile::SetBlocked(bool status) noexcept
{
	m_isBlocked = status;
}

const SDL_Rect& ecc::Tile::GetCurrentRect() const noexcept
{
	return m_lit ? m_texCoordOn : m_texCoordOff;
}

const SDL_Rect& ecc::Tile::GetDestinationLocation() const noexcept
{
	return m_destLocation;
}

bool ecc::Tile::IsClimbable() const noexcept
{
	return m_isClimbable;
}

bool ecc::Tile::IsWalkable() const noexcept
{
	return m_isWalkable;
}

bool ecc::Tile::IsBlocked() const noexcept
{
	return m_isBlocked;
}

void ecc::Tile::SetCollisionBox()
{
	m_collisionBox.x = m_destLocation.x;
	m_collisionBox.y = m_destLocation.y;
	m_collisionBox.w = m_destLocation.w;
	m_collisionBox.h = m_destLocation.h;
}

void ecc::Tile::SetWalkableCollisionBox()
{
	m_collisionBox.x = m_destLocation.x - (m_destLocation.w / 2);
	m_collisionBox.y = m_destLocation.y - (m_destLocation.h / 2);
	m_collisionBox.w = m_destLocation.w * 2;
	m_collisionBox.h = m_destLocation.h * 2;
}
