#include "ObjectFactory.h"

ecc::ObjectFactory::ObjectFactory(SDL_Renderer* renderer)
{
	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Box, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Box]->LoadObject(renderer, SPRITE_BOX);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::DoorBlocked, std::make_unique<Image>()));
	m_objectMappings[ObjectList::DoorBlocked]->LoadObject(renderer, SPRITE_DOOR_BLOCKED);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::DoorUnblocked, std::make_unique<Image>()));
	m_objectMappings[ObjectList::DoorUnblocked]->LoadObject(renderer, SPRITE_DOOR_UNBLOCKED);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::LeverOff, std::make_unique<Image>()));
	m_objectMappings[ObjectList::LeverOff]->LoadObject(renderer, SPRITE_LEVER_OFF);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::LeverOn, std::make_unique<Image>()));
	m_objectMappings[ObjectList::LeverOn]->LoadObject(renderer, SPRITE_LEVER_ON);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window0, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window0]->LoadObject(renderer, SPRITE_WINDOW_0);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window1Closed, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window1Closed]->LoadObject(renderer, SPRITE_WINDOW_1_CLOSED);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window1Open, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window1Open]->LoadObject(renderer, SPRITE_WINDOW_1_OPEN);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window2, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window2]->LoadObject(renderer, SPRITE_WINDOW_2);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window3, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window3]->LoadObject(renderer, SPRITE_WINDOW_3);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window3Closed, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window3Closed]->LoadObject(renderer, SPRITE_WINDOW_3_CLOSED);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window3Open, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window3Open]->LoadObject(renderer, SPRITE_WINDOW_3_OPEN);

	m_objectMappings.insert(std::pair<ObjectList, std::unique_ptr<Image>>(ObjectList::Window4, std::make_unique<Image>()));
	m_objectMappings[ObjectList::Window4]->LoadObject(renderer, SPRITE_WINDOW_4);
}

ecc::ObjectFactory::~ObjectFactory()
{
	IRenderable::~IRenderable();
}

void ecc::ObjectFactory::SetCollisionBox()
{
	m_collisionBox.w = m_image->m_width;
	m_collisionBox.h = m_image->m_height;
	m_collisionBox.x = m_image->m_destinationLocation.x;
	m_collisionBox.y = m_image->m_destinationLocation.y;
}

void ecc::ObjectFactory::Render(SDL_Renderer* renderer, float speedFactor)
{
	m_image->Render(renderer);
}

const SDL_Rect& ecc::ObjectFactory::GetCurrentDestination()
{
	return m_image->m_destinationLocation;
}

void ecc::ObjectFactory::CreateWindow(SDL_Renderer* renderer, ecc::ObjectList type, int xPos, int yPos)
{
	m_objectMappings[type]->RenderObject(renderer, xPos, yPos);
}

void ecc::ObjectFactory::CreateKeyOrDoor(SDL_Renderer* renderer, ObjectList type, int xPos, int yPos)
{
	m_objectMappings[type]->RenderObject(renderer, xPos, yPos);
}

void ecc::ObjectFactory::GetImageDimension(ObjectList type, int& width, int& height)
{
	width = m_objectMappings[type]->m_width * 2;
	height = m_objectMappings[type]->m_height * 2;
}
