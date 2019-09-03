#pragma once
#include "ICollidable.h"
#include "IRenderable.h"
#include "ObjectFactory.h"

namespace ecc {
	class SwitchableWindow : public ICollidable, public IRenderable
	{
	private:
		ObjectList m_onType = {};
		ObjectList m_offType = {};
		int m_xPos = 0;
		int m_yPos = 0;
		bool m_lit = true;
		int m_group = 0;
		int m_xLightingRange = 0;
		int m_yLightingRange = 0;
		bool m_enable = true;

	public:
		SwitchableWindow(ObjectList onType, ObjectList offType, int xPos, int yPos,
			int xRange, int yRange,
			bool isLit = true, int group = 0);

		// ICollidable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void SetCollisionBox() override;
		void SetCollisionBox(ObjectFactory* factory);

		// IRenderable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void Render(SDL_Renderer* renderer, float speedFactor) override;
		void Render(SDL_Renderer* renderer, ObjectFactory* factory);
		
		void Switch(int group = 0);
		void SetEnable(bool status);

		virtual const SDL_Rect& GetCurrentDestination() override;
		const SDL_Rect& GetCurrentDestination(ObjectFactory* factory);
		bool IsLit() const noexcept;
		void GetLightingRange(int& x, int& y);
	};
};