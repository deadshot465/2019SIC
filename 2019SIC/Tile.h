#pragma once
#include <SDL.h>
#include <vector>
#include "Helper.h"

namespace ecc {
	class Tile
	{
	protected:
		SDL_Rect m_texCoordOn = {};
		SDL_Rect m_texCoordOff = {};
		SDL_Rect m_destLocation = {};
		TileType m_tileType = {};
		bool m_lit = false;

	public:
		Tile(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff,
			const SDL_Rect& destLocation, TileType tileType, bool isLit);
		~Tile();

		bool IsLit() const noexcept;
		void ChangeStatus(bool isLit);

		const SDL_Rect& GetCurrentRect() const noexcept;
		const SDL_Rect& GetDestinationLocation() const noexcept;
	};
}
