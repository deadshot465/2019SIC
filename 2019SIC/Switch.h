#pragma once
#include "Tile.h"

namespace ecc {
	class Switch :
		public Tile
	{
	private:
		int m_xRange = 0;
		int m_yRange = 0;

	public:
		Switch(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff, const SDL_Rect& destLocation, TileType tileType, bool isLit,
			int xRange, int yRange);
		~Switch();

		int GetXRange() const noexcept;
		int GetYRange() const noexcept;
	};
}
