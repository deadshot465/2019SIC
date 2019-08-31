#include "GameEngine.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <SDL_image.h>
#include <sstream>
#include "Switch.h"

ecc::TileCoordinatesSet ecc::GameEngine::CalculateTileCoordinates(int totalWidth, int totalHeight)
{
	auto tile_coordinates = TileCoordinatesSet();
	int total_columns = totalWidth / TILE_WIDTH;
	int total_rows = totalHeight / TILE_HEIGHT;

	tile_coordinates.resize(total_rows);

	int x = 0;
	int y = 0;
	for (auto& row : tile_coordinates) {
		row.resize(total_columns);
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

void ecc::GameEngine::UpdateCharacters()
{
	if (m_characterIndex == 0) {
		m_father->Move();
	}
	else {
		m_daughter->Move();
	}

	switch (m_father->GetCurrentStatus()) {
	case ImageIndexFlag::Idle:
		m_father->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 5.0f);
		break;
	case ImageIndexFlag::Moving:
		m_father->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 0.25f);
		break;
	case ImageIndexFlag::Attacking:
		m_father->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 1.0f);
		break;
	}

	switch (m_daughter->GetCurrentStatus()) {
	case ImageIndexFlag::Idle:
		m_daughter->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 3.0f);
		break;
	case ImageIndexFlag::Moving:
		m_daughter->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 1.0f);
		break;
	case ImageIndexFlag::Attacking:
		m_daughter->Render(m_renderer, m_camera->GetCollisionBox().x,
			m_camera->GetCollisionBox().y, 0.5f);
		break;
	}

	for (const auto& object : m_objects) {
		auto res_1 = m_father->CheckCollision(object->GetCollisionBox());
		auto res_2 = m_daughter->CheckCollision(object->GetCollisionBox());
	}
}

void ecc::GameEngine::CreateTiles(size_t imageIndex, int totalWidth, int totalHeight,
	SDL_Surface* windowSurface)
{
	auto src_rect_on = SDL_Rect();
	auto src_rect_off = SDL_Rect();
	auto dst_rect = SDL_Rect();
	bool is_lit = false;

	auto tex_coord_index = m_tileCoordinateMapping[imageIndex];
	auto tex_coord = m_tileCoordinates[tex_coord_index];
	auto total_rows = tex_coord.size();
	auto total_columns = tex_coord[0].size();
	int x = 0;
	int y = 0;
	auto tiles = std::vector<std::shared_ptr<Tile>>();

	for (const auto& row : m_maps[imageIndex]) {
		for (const auto& column : row) {
			for (auto current_row = static_cast<int64_t>(total_rows) - 2ll; current_row >= 0ll; current_row -= 2ll) {

				auto offset = current_row * total_columns;
				if (column < offset) continue;

				if (column > offset + total_columns) {
					src_rect_off = tex_coord[current_row + 1ll][column % 10];
					src_rect_on = tex_coord[current_row][column % 10];
					is_lit = false;
					break;
				}
				else {
					src_rect_off = tex_coord[current_row + 1ll][column % 10];
					src_rect_on = tex_coord[current_row][column % 10];
					is_lit = true;
					break;
				}
			}
			dst_rect.x = x * TILE_WIDTH;
			//dst_rect.x = x * (windowSurface->w / m_currentMapWidth * TILE_WIDTH);
			dst_rect.y = y * TILE_HEIGHT;
			//dst_rect.y = y * (windowSurface->h / m_currentMapHeight * TILE_HEIGHT);
			dst_rect.w = TILE_WIDTH;
			dst_rect.h = TILE_HEIGHT;
			++x;
			tiles.emplace_back(std::make_shared<Tile>(src_rect_on, src_rect_off, dst_rect, TileType::Normal, is_lit));
		}
		x = 0;
		++y;
	}
	y = 0;
	m_tileSet.emplace_back(tiles);

	/*for (auto j = 0; j < m_currentMapHeight; ++j) {
		for (auto k = 0; k < m_currentMapWidth; ++k) {
			auto tile = m_map[j * m_currentMapWidth + k];

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
	}*/
}

void ecc::GameEngine::RenderTiles(SDL_Surface* windowSurface, const SDL_Rect& cameraRect)
{
	Uint32 format = 0;
	int access = 0;
	int width = 0;
	int height = 0;
	SDL_QueryTexture(m_defaultRtv, &format, &access, &width, &height);
	m_tileRtv = SDL_CreateTexture(m_renderer, format, SDL_TEXTUREACCESS_TARGET,
		m_currentMapWidth * TILE_WIDTH,
		m_currentMapHeight * TILE_HEIGHT);
	SDL_SetRenderTarget(m_renderer, m_tileRtv);
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);

	for (size_t i = 0; i < m_tileSet.size(); ++i) {
		
		auto image_index_iter = std::find_if(m_tileCoordinateMapping.cbegin(),
			m_tileCoordinateMapping.cend(), [&i](const std::pair<size_t, size_t>& element) {
				return element.second == i;
			});
		auto image_index = image_index_iter->first;

		for (const auto& tile : m_tileSet[i]) {
			SDL_RenderCopy(m_renderer, m_images[image_index]->m_texture, &(tile->GetCurrentRect()),
				&(tile->GetDestinationLocation()));
		}

	}
	SDL_SetRenderTarget(m_renderer, nullptr);

	//if (!m_tileRendered) {

	//	for (size_t i = 0; i < m_tileSet.size(); ++i) {
	//		Uint32 format = 0;
	//		int access = 0;
	//		int width = 0;
	//		int height = 0;

	//		auto image_index_iter = std::find_if(m_tileCoordinateMapping.cbegin(),
	//			m_tileCoordinateMapping.cend(), [&i](const std::pair<size_t, size_t>& element) {
	//				return element.second == i;
	//			});
	//		auto image_index = image_index_iter->first;

	//		SDL_QueryTexture(m_images[image_index]->m_texture, &format, &access, &width, &height);
	//		auto rtv = SDL_CreateTexture(m_renderer, format, SDL_TEXTUREACCESS_TARGET,
	//			m_currentMapWidth * TILE_WIDTH,
	//			m_currentMapHeight * TILE_HEIGHT);
	//		SDL_SetRenderTarget(m_renderer, rtv);
	//		if (i == 0) SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//		SDL_RenderClear(m_renderer);

	//		for (const auto& tile : m_tileSet[i]) {
	//			SDL_RenderCopy(m_renderer, m_images[image_index]->m_texture, &(tile->GetCurrentRect()),
	//				&(tile->GetDestinationLocation()));
	//		}

	//		m_tileRtvs.emplace_back(rtv);
	//		SDL_SetRenderTarget(m_renderer, nullptr);
	//	}
	//	m_tileRendered = true;
	//	

	//	/*for (const auto& tile : m_tiles) {

	//		if (tile->GetDestinationLocation().x > cameraRect.x &&
	//			tile->GetDestinationLocation().x < cameraRect.x + cameraRect.w &&
	//			tile->GetDestinationLocation().y > cameraRect.y &&
	//			tile->GetDestinationLocation().y < cameraRect.y + cameraRect.h) {
	//			SDL_RenderCopy(m_renderer, m_images[0]->m_texture, &(tile->GetCurrentRect()),
	//				&(tile->GetDestinationLocation()));
	//		}

	//		
	//	}*/
	//}
	//else {
	//	auto dst_rect = SDL_Rect();
	//	dst_rect.x = 0;
	//	dst_rect.y = 0;
	//	//dst_rect.w = m_currentMapWidth * TILE_WIDTH;
	//	//dst_rect.h = m_currentMapHeight * TILE_HEIGHT;
	//	dst_rect.w = windowSurface->w;
	//	dst_rect.h = windowSurface->h;
	//	for (const auto& rtv : m_tileRtvs) {
	//		SDL_RenderCopy(m_renderer, rtv, nullptr, &dst_rect);
	//	}
	//}
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

int ecc::GameEngine::GetSingleDirectionMoveBound(const SDL_Rect& nextArea, int increment)
{
	if (GetTile(nextArea)->IsLit())
		return nextArea.x;
	
	auto rect = SDL_Rect();
	rect.x = nextArea.x + increment;
	rect.y = nextArea.y;
	rect.w = nextArea.w;
	rect.h = nextArea.h;
	return GetSingleDirectionMoveBound(rect, increment);
}

void ecc::GameEngine::GetEnemyMoveBounds(Enemy* enemy, int& leftBound, int& rightBound)
{
	auto current_location = enemy->GetCurrentDestination();
	leftBound = GetSingleDirectionMoveBound({
		current_location.x,
		current_location.y,
		current_location.w,
		current_location.h
		}, -TILE_WIDTH) / TILE_WIDTH;
	leftBound -= ENEMY_SPRITE_OFFSET;

	rightBound = GetSingleDirectionMoveBound({
		current_location.x,
		current_location.y,
		current_location.w,
		current_location.h
		}, TILE_WIDTH) / TILE_WIDTH;
	rightBound -= ENEMY_SPRITE_OFFSET;
}

void ecc::GameEngine::MoveEnemy()
{
	static int next_destination = 0;

	for (auto& enemy : m_enemies) {
		if (enemy->IsMoveFinished()) {
			int left_bound = 0;
			int right_bound = 0;
			GetEnemyMoveBounds(enemy.get(), left_bound, right_bound);

			while (next_destination == 0)
				next_destination = GetRandomInt(left_bound, right_bound);

			enemy->StartMove();
		}
		enemy->Move(next_destination);
		enemy->Render(m_renderer);
	}
}

ecc::GameEngine::GameEngine(SDL_Window* window, SDL_Surface* windowSurface)
{
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	m_defaultRtv = SDL_GetRenderTarget(m_renderer);
	m_camera = std::make_unique<Camera>(windowSurface);
}

ecc::GameEngine::~GameEngine()
{
	SDL_DestroyTexture(m_tileRtv);
	SDL_DestroyTexture(m_defaultRtv);
	SDL_DestroyRenderer(m_renderer);
	IMG_Quit();
}

void ecc::GameEngine::LoadImage(const std::string& fileName,
	SDL_Surface* windowSurface,
	bool isTileSet)
{
	size_t index = m_images.size();
	m_images.emplace_back(std::make_unique<Image>());
	auto iter = m_images.end();
	--iter;
	iter->get()->LoadImage(m_renderer, fileName, true, 0, 0, isTileSet);

	if (isTileSet) {
		auto coordinates = CalculateTileCoordinates(iter->get()->m_width, iter->get()->m_height);
		m_tileCoordinateMapping.insert(
			std::pair<size_t, size_t>(index, m_tileCoordinates.size()));
		m_tileCoordinates.emplace_back(coordinates);
		CreateTiles(index, iter->get()->m_width, iter->get()->m_height, windowSurface);
	}

}

void ecc::GameEngine::LoadObject(const std::string& fileName, int xPos, int yPos)
{
	m_objects.emplace_back(std::make_unique<Object>(m_renderer, fileName, xPos, yPos));
}

void ecc::GameEngine::LoadMap(const std::string& mapName, const std::string& fileName,
	SDL_Surface* windowSurface)
{
	auto fs = std::ifstream();

	fs.open(mapName, std::ios_base::in | std::ios_base::beg);

	auto number_array = std::vector<std::vector<int>>();

	if (fs.good()) {
		do
		{
			char* buffer = new char[500];
			auto ss = std::stringstream();
			fs.getline(buffer, 499);

			ss.clear();
			ss << buffer;

			std::vector<int> numbers;
			do {
				int x = 0;
				ss >> x;
				
				if (ss.fail()) {
					ss.clear();
					long long location = ss.tellg();
					ss.seekg(location + 1);
				}
				else {
					numbers.emplace_back(x);
				}

				if (ss.eof()) {
					break;
				}
				
			} while (!ss.fail());
			
			if (fs.fail()) {
				if (buffer[0] == '\0') {
					fs.setstate(std::ios_base::eofbit);
				}
				else {
					fs.clear();
					long long location = fs.tellg();
					fs.seekg(location + 1);
				}
			}
			else {
				number_array.emplace_back(numbers);
				delete[] buffer;
			}

			if (fs.eof())
				break;
			
		} while (!fs.fail());
			
	}

	fs.close();
	m_maps.emplace_back(number_array);
	m_currentMapWidth = number_array[0].size();
	m_currentMapHeight = number_array.size();
	LoadImage(fileName, windowSurface, true);
}

void ecc::GameEngine::LoadCharacter(const std::string& waitAnimationFileName,
	const std::string& moveAnimationFileName,
	const std::string& attackAnimationFileName,
	Character::CharacterFlag characterFlag,
	int xPos, int yPos, float speed, ImageIndexFlag initialStatus)
{
	switch (characterFlag)
	{
	case ecc::Character::CharacterFlag::Father:
		m_father = std::make_unique<Character>(m_renderer,
			waitAnimationFileName, moveAnimationFileName,
			attackAnimationFileName,
			characterFlag,
			xPos, yPos, speed, initialStatus);
		break;
	case ecc::Character::CharacterFlag::Daughter:
		m_daughter = std::make_unique<Character>(m_renderer,
			waitAnimationFileName, moveAnimationFileName,
			attackAnimationFileName,
			characterFlag,
			xPos, yPos, speed, initialStatus);
		break;
	default:
		break;
	}

}

void ecc::GameEngine::LoadEnemy(const std::string& waitAnimationFileName, const std::string& moveAnimationFileName, int xPos, int yPos, float speed)
{
	m_enemies.emplace_back(std::make_unique<Enemy>(m_renderer,
		waitAnimationFileName, moveAnimationFileName, xPos, yPos, speed));
}

void ecc::GameEngine::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	SDL_RenderClear(m_renderer);
}

void ecc::GameEngine::Render(SDL_Surface* windowSurface, float scaleX, float scaleY)
{
	int camera_x_pos = 0;
	int camera_y_pos = 0;
	if (m_characterIndex == 0) {
		camera_x_pos = (m_father->GetCurrentDestination().x + (CHARACTER_SPRITE_WIDTH / 2)) - (m_camera->GetCollisionBox().w / 2);
		camera_y_pos = (m_father->GetCurrentDestination().y + (CHARACTER_SPRITE_HEIGHT / 2)) - (m_camera->GetCollisionBox().h / 2);
	}
	else {
		camera_x_pos = (m_daughter->GetCurrentDestination().x + (CHARACTER_SPRITE_WIDTH / 2)) - (m_camera->GetCollisionBox().w / 2);
		camera_y_pos = (m_daughter->GetCurrentDestination().y + (CHARACTER_SPRITE_HEIGHT / 2)) - (m_camera->GetCollisionBox().h / 2);
	}
	m_camera->SetCollisionBox(camera_x_pos, camera_y_pos, m_currentMapWidth * TILE_WIDTH, m_currentMapHeight * TILE_HEIGHT);

	if (!m_cameraRtv) {
		Uint32 format = 0;
		int access = 0;
		int w = 0;
		int h = 0;
		SDL_QueryTexture(m_defaultRtv, &format, &access, &w, &h);
		m_cameraRtv = SDL_CreateTexture(m_renderer, format, SDL_TEXTUREACCESS_TARGET,
			m_camera->GetCollisionBox().w, m_camera->GetCollisionBox().h);
	}

	//SDL_RenderSetScale(m_renderer, scaleX, scaleY);
	RenderTiles(windowSurface, m_camera->GetCollisionBox());
	//SDL_RenderCopy(m_renderer, m_tileRtv, nullptr, nullptr);

	SDL_SetRenderTarget(m_renderer, m_cameraRtv);
	//SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_tileRtv,
		&(m_camera->GetCollisionBox()),
		nullptr);
	
	/*for (auto i = 0; i < m_images.size(); ++i) {
		if (m_images[i]->m_isTile)
			continue;

		m_images[i]->Render(m_renderer);
	}

	for (const auto& object : m_objects) {
		object->Render(m_renderer);
	}*/

	UpdateCharacters();

	//SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0x00, 0xFF);
	//SDL_RenderDrawRect(m_renderer, &(m_camera->GetCollisionBox()));
	//SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(m_renderer, m_cameraRtv, nullptr, nullptr);
	

	//MoveEnemy();

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
	if (m_characterIndex == 0 &&
		m_daughter->GetCurrentStatus() == ImageIndexFlag::Idle) {
		m_characterIndex = 1;
		m_father->SetStatus(ImageIndexFlag::Idle);
		m_daughter->SetStatus(ImageIndexFlag::Moving);
	}
	else if (m_characterIndex == 1 &&
		m_father->GetCurrentStatus() == ImageIndexFlag::Idle) {
		m_characterIndex = 0;
		m_father->SetStatus(ImageIndexFlag::Moving);
		m_daughter->SetStatus(ImageIndexFlag::Idle);
	}
}

unsigned short ecc::GameEngine::GetCharacterIndex() const noexcept
{
	return m_characterIndex;
}
