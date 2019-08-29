#include "GameEngine.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <SDL_image.h>
#include "Switch.h"

ecc::TileCoordinatesSet ecc::GameEngine::CalculateTileCoordinates()
{
	auto tile_coordinates = TileCoordinatesSet();
	int x = 0;
	int y = 0;
	for (auto& row : tile_coordinates) {
		for (auto& column : row) {
			column.x = x * TILE_WIDTH;
			column.y = y * TILE_HEIGHT;
			column.w = TILE_WIDTH;
			column.h = TILE_HEIGHT;
			++x;
		}
		x = 0;
		++y;
	}

	return tile_coordinates;
}

void ecc::GameEngine::CreateTiles(size_t imageIndex)
{
	auto src_rect_on = SDL_Rect();
	auto src_rect_off = SDL_Rect();
	auto dst_rect = SDL_Rect();
	bool is_lit = false;

	for (auto j = 0; j < MAX_MAP_Y; ++j) {
		for (auto k = 0; k < MAX_MAP_X; ++k) {
			auto tile = m_map[j * MAX_MAP_X + k];
			auto tex_coord_index = m_tileCoordinateSet[imageIndex];
			auto tex_coord = m_tileCoordinates[tex_coord_index];

			if (tile < MAX_TILE_COUNT_X) {
				src_rect_on = tex_coord[0][tile];
				src_rect_off = tex_coord[1][tile];
				is_lit = true;
			}
			else {
				src_rect_on = tex_coord[0][tile - MAX_TILE_COUNT_X];
				src_rect_off = tex_coord[1][tile - MAX_TILE_COUNT_X];
				is_lit = false;
			}

			dst_rect.x = k * TILE_WIDTH;
			dst_rect.y = j * TILE_HEIGHT;
			dst_rect.w = TILE_WIDTH;
			dst_rect.h = TILE_HEIGHT;

			TileType type = (tile == 5) ? TileType::Switch : TileType::Normal;

			switch (type) {
			case TileType::Normal:
				m_tiles.emplace_back(std::make_unique<Tile>(src_rect_on, src_rect_off, dst_rect, type, is_lit));
				break;
			case TileType::Switch:
			{
				m_switchIndices.emplace_back(m_tiles.size());
				m_tiles.emplace_back(std::make_unique<Switch>(src_rect_on, src_rect_off, dst_rect,
					type, is_lit, 3, 8));
				break;
			}
			}
		}
	}
}

void ecc::GameEngine::RenderTiles(const SDL_Rect& cameraRect)
{
	for (const auto& tile : m_tiles) {

		/*if (tile->GetDestinationLocation().x > cameraRect.x &&
			tile->GetDestinationLocation().x < cameraRect.x + cameraRect.w &&
			tile->GetDestinationLocation().y > cameraRect.y &&
			tile->GetDestinationLocation().y < cameraRect.y + cameraRect.h) {
			SDL_RenderCopy(m_renderer, m_images[0]->m_texture, &(tile->GetCurrentRect()),
				&(tile->GetDestinationLocation()));
		}*/

		SDL_RenderCopy(m_renderer, m_images[0]->m_texture, &(tile->GetCurrentRect()),
			&(tile->GetDestinationLocation()));
	}
}

ecc::Tile* ecc::GameEngine::GetTile(const SDL_Rect& location) noexcept
{
	for (const auto& tile : m_tiles) {
		if (tile->GetDestinationLocation().x == location.x &&
			tile->GetDestinationLocation().y == location.y) {
			return tile.get();
		}
	}

	return nullptr;
}

ecc::GameEngine::GameEngine(SDL_Window* window)
{
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	m_camera = std::make_unique<Camera>();
}

ecc::GameEngine::~GameEngine()
{
	SDL_DestroyRenderer(m_renderer);
	IMG_Quit();
}

void ecc::GameEngine::LoadImage(const std::string& fileName, bool isTileSet)
{
	size_t index = m_images.size();
	m_images.emplace_back(std::make_unique<Image>());
	auto iter = m_images.end();
	--iter;
	iter->get()->LoadImage(m_renderer, fileName, true, 0, 0, isTileSet);

	if (isTileSet) {
		auto coordinates = CalculateTileCoordinates();
		m_tileCoordinateSet.insert(
			std::pair<size_t, size_t>(index, m_texCoordIndices.size()));
		m_tileCoordinates.emplace_back(coordinates);
		CreateTiles(index);
	}
		
}

void ecc::GameEngine::LoadObject(const std::string& fileName, int xPos, int yPos)
{
	m_objects.emplace_back(std::make_unique<Object>(m_renderer, fileName, xPos, yPos));
}

void ecc::GameEngine::LoadMap(const std::string& mapName)
{
	auto fs = std::ifstream();

	fs.open(mapName, std::ios_base::in | std::ios_base::beg);

	int temp = 0;

	if (fs.good()) {
		do
		{
			int temp = 0;
			fs >> temp;
			temp -= 1;
			
			if (fs.eof())
				break;

			if (fs.fail()) {
				fs.clear();
				long long location = fs.tellg();
				fs.seekg(location + 1);
			}
			else {
				m_map.emplace_back(temp);
			}
		} while (!fs.fail());
			
	}

	fs.close();
}

void ecc::GameEngine::LoadCharacter(const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	int xPos, int yPos, float speed)
{
	m_characters.emplace_back(std::make_unique<Character>(m_renderer,
		waitAnimationFileName, moveAnimationFileName, xPos, yPos, speed));
}

void ecc::GameEngine::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a, float scaleX, float scaleY)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	SDL_RenderClear(m_renderer);
	SDL_RenderSetScale(m_renderer, scaleX, scaleY);
}

void ecc::GameEngine::Render()
{
	auto camera_x_pos = m_characters[m_characterIndex]->GetCurrentDestination().x - CHARACTER_SPRITE_WIDTH;
	auto camera_y_pos = m_characters[m_characterIndex]->GetCurrentDestination().y - CHARACTER_SPRITE_HEIGHT;

	m_camera->SetCollisionBox(camera_x_pos, camera_y_pos);

	RenderTiles(m_camera->GetCollisionBox());
	
	for (auto i = 0; i < m_images.size(); ++i) {
		if (m_images[i]->m_isTile)
			continue;

		m_images[i]->Render(m_renderer);
	}

	for (const auto& object : m_objects) {
		object->Render(m_renderer);
	}

	for (auto i = 0; i < m_characters.size(); ++i) {

		if (i == m_characterIndex)
			m_characters[i]->Move();

		m_characters[i]->Render(m_renderer);
	}

	for (const auto& character : m_characters) {
		for (const auto& object : m_objects) {
			auto res = character->CheckCollision(object->GetCollisionBox());
			if (res)
				std::cout << "Collision Detected.\n";
		}
	}

	SDL_RenderPresent(m_renderer);
}

void ecc::GameEngine::SwitchLight()
{
	for (const auto& index : m_switchIndices) {
		auto switch_ = reinterpret_cast<Switch*>(m_tiles[index].get());
		auto x_range = switch_->GetXRange();
		auto y_range = switch_->GetYRange();
		auto location = switch_->GetDestinationLocation();

		Tile* target_y = nullptr;
		Tile* target_x_left = nullptr;
		Tile* target_x_right = nullptr;

		for (int i = y_range; i > 0; --i) {
			target_y = GetTile({
				location.x,
				location.y + TILE_HEIGHT * i,
				TILE_WIDTH,
				TILE_HEIGHT });
			
			for (int j = x_range; j > 0; --j) {
				target_x_left = GetTile({
					location.x - TILE_WIDTH * j,
					target_y->GetDestinationLocation().y,
					TILE_WIDTH, TILE_HEIGHT });
				target_x_right = GetTile({
					location.x + TILE_WIDTH * j,
					target_y->GetDestinationLocation().y,
					TILE_WIDTH, TILE_HEIGHT });

				target_x_left->ChangeStatus(!target_x_left->IsLit());
				target_x_right->ChangeStatus(!target_x_right->IsLit());
			}

			target_y->ChangeStatus(!target_y->IsLit());
		}
	}
}

void ecc::GameEngine::SetCharacterIndex(unsigned short index)
{
	m_characters[m_characterIndex]->SetStatus(0);
	m_characterIndex = index;
	m_characters[m_characterIndex]->SetStatus(1);
}

unsigned short ecc::GameEngine::GetCharacterIndex() const noexcept
{
	return m_characterIndex;
}
