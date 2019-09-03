#pragma once
#include <SDL.h>
#include <vector>
#include "Helper.h"
#include "ICollidable.h"

namespace ecc {
	class Tile : public ICollidable
	{
	protected:
		SDL_Rect m_texCoordOn = {};
		SDL_Rect m_texCoordOff = {};
		SDL_Rect m_destLocation = {};
		TileType m_tileType = {};
		bool m_lit = false;
		bool m_isClimbable = false;
		bool m_isWalkable = false;
		bool m_isBlocked = false;

	public:
		Tile(const SDL_Rect& texCoordOn, const SDL_Rect& texCoordOff,
			const SDL_Rect& destLocation, TileType tileType, bool isLit, bool isClimbable, bool isWalkable, bool isBlocked);
		~Tile();

		bool IsLit() const noexcept;
		void ChangeStatus(bool isLit) noexcept;
		void SetClimbable(bool status) noexcept;
		void SetWalkable(bool status) noexcept;
		void SetBlocked(bool status) noexcept;

		const SDL_Rect& GetCurrentRect() const noexcept;
		const SDL_Rect& GetDestinationLocation() const noexcept;
		
		bool IsClimbable() const noexcept;
		bool IsWalkable() const noexcept;
		bool IsBlocked() const noexcept;

		// ICollidable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void SetCollisionBox() override;
		void SetWalkableCollisionBox();
	};
}
