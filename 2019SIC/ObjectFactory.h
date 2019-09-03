#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "ICollidable.h"
#include "Image.h"
#include "IRenderable.h"

namespace ecc {
	class ObjectFactory : public ICollidable, public IRenderable
	{
	private:
		inline static const std::string SPRITE_BOX = "texture/box.png";
		inline static const std::string SPRITE_DOOR_BLOCKED = "texture/door_new(blocked).png";
		inline static const std::string SPRITE_DOOR_UNBLOCKED = "texture/door_new.png";
		inline static const std::string SPRITE_LEVER_OFF = "texture/lever_off.png";
		inline static const std::string SPRITE_LEVER_ON = "texture/lever_on.png";
		inline static const std::string SPRITE_WINDOW_0 = "texture/window.png";
		inline static const std::string SPRITE_WINDOW_1_CLOSED = "texture/window-1_closed.png";
		inline static const std::string SPRITE_WINDOW_1_OPEN = "texture/window-1_open.png";
		inline static const std::string SPRITE_WINDOW_2 = "texture/window-2.png";
		inline static const std::string SPRITE_WINDOW_3 = "texture/window-3.png";
		inline static const std::string SPRITE_WINDOW_3_CLOSED = "texture/window-3_closed.png";
		inline static const std::string SPRITE_WINDOW_3_OPEN = "texture/window-3_open.png";
		inline static const std::string SPRITE_WINDOW_4 = "texture/window-4.png";
		
		std::unordered_map<ObjectList, std::unique_ptr<Image>> m_objectMappings;
	public:
		ObjectFactory(SDL_Renderer* renderer);
		~ObjectFactory();

		// ICollidable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual void SetCollisionBox() override;

		virtual void Render(SDL_Renderer* renderer, float speedFactor = 1.0f) override;

		// IRenderable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual const SDL_Rect& GetCurrentDestination() override;

		void CreateWindow(SDL_Renderer* renderer, ObjectList type, int xPos, int yPos);
		void CreateKeyOrDoor(SDL_Renderer* renderer, ObjectList type, int xPos, int yPos);

		void GetImageDimension(ObjectList type, int& width, int& height);
	};
}
