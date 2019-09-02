#include "Switch.h"

ecc::Switch::Switch(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff, const SDL_Rect& destLocation, TileType tileType, bool isLit, int xRange, int yRange) : Tile(texCoordOn, texCoordOff,
	destLocation, tileType, isLit), m_xRange(xRange), m_yRange(yRange)
{
}

ecc::Switch::~Switch()
{
}

int ecc::Switch::GetXRange() const noexcept
{
	return m_xRange;
}

int ecc::Switch::GetYRange() const noexcept
{
	return m_yRange;
}