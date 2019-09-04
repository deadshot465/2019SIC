#include "GameEngine.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <SDL_image.h>
#include <set>
#include <sstream>

#if _DEBUG
#else
#define NDEBUG
#endif

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

void ecc::GameEngine::GetAllWalkableTiles()
{
	size_t index = (m_tileSet.size() >= 2) ? 1 : 0;
	for (const auto& tile : m_tileSet[index]) {
		if (tile->IsWalkable()) {
			m_walkableTiles.emplace_back(tile->GetCollisionBox());
		}
	}
}

void ecc::GameEngine::GetAllBlockedTiles()
{
	size_t index = (m_tileSet.size() >= 2) ? 1 : 0;
	for (const auto& tile : m_tileSet[index]) {
		if (tile->IsBlocked()) {
			m_blockedTiles.emplace_back(tile->GetCollisionBox());
		}
	}
}

void ecc::GameEngine::GetAllClimbableTiles()
{
	size_t index = (m_tileSet.size() >= 2) ? 1 : 0;
	for (const auto& tile : m_tileSet[index]) {
		if (tile->IsClimbable()) {
			m_climbableTiles.emplace_back(tile->GetCollisionBox());
		}
	}
}

void ecc::GameEngine::UpdateCharacters(int offsetX, int offsetY, SDL_Surface* windowSurface)
{
	if (!m_father || !m_daughter) return;

	auto father_position = m_father->GetCurrentDestination();
	auto daughter_position = m_daughter->GetCurrentDestination();

	if (m_characterIndex == 0) {
		m_father->Jump();
		m_father->Move(m_mixer.get(), windowSurface);
	}
	else {
		m_daughter->Jump();
		m_daughter->Move(m_mixer.get(), windowSurface);
	}

	m_daughter->Move();

	m_father->Render(m_renderer, offsetX,
		offsetY, 0.0f);

	m_daughter->Render(m_renderer, offsetX,
		offsetY, 0.0f);

	/*for (const auto& key : m_keys) {

		if (!key->GetActiveStatus()) continue;

		bool result = false;
		if (m_characterIndex == 0) {
			result = m_father->CheckCollision(key->GetCollisionBox());
		}
		else {
			result = m_daughter->CheckCollision(key->GetCollisionBox());
		}
		
		if (result) {
			key->SetActive(false);
		}
	}*/

	/*for (const auto& window : m_switchableWindows) {
		if (m_father->CheckCollision(window->GetCollisionBox()) && window->IsLit()) {
			m_currentGameStatus = GameStatus::GameOver;
			break;
		}
	}*/

	auto iter = m_enemies.begin();
	while (iter != m_enemies.end()) {
		if (m_father->CheckAttackCollision(iter->get()->GetCollisionBox())) {
			iter = m_enemies.erase(iter);
			++m_combo;
			m_mixer->PlaySound(SoundList::Strike, 1);
		}
		else {
			++iter;
		}
	}

	if (m_combo >= 5) {
		m_combo = 0;
		m_hp += 50;
		m_hp = std::clamp<int>(m_hp, 0, 500);
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

				if (column >= offset + total_columns) {
					src_rect_off = tex_coord[current_row + 1ll][column % total_columns];
					src_rect_on = tex_coord[current_row][column % total_columns];
					is_lit = false;
					break;
				}
				else {
					src_rect_off = tex_coord[current_row + 1ll][column % total_columns];
					src_rect_on = tex_coord[current_row][column % total_columns];
					is_lit = true;
					break;
				}
			}
			dst_rect.x = x * TILE_WIDTH * 2;
			dst_rect.y = y * TILE_HEIGHT * 2;
			dst_rect.w = TILE_WIDTH * 2;
			dst_rect.h = TILE_HEIGHT * 2;
			++x;

			bool climbable = false;
			bool walkable = false;
			bool blocked = false;

			if (column == 0 || column == 58 || column == 12 || column == 70) {
				walkable = true;
			}
			/*if (imageIndex == 1) {
				if (column == 54 || column == 55 || column == 66 || column == 67) {
					climbable = true;
				}
				if (column == 1 || column == 6 || column == 7 || column == 13 || column == 18 || column == 19) {
					blocked = true;
				}
			}*/

			auto tile = std::make_shared<Tile>(src_rect_on, src_rect_off, dst_rect,
				TileType::Normal, is_lit, climbable, walkable, blocked);

			tiles.emplace_back(tile);
		}
		x = 0;
		++y;
	}
	y = 0;
	m_tileSet.emplace_back(tiles);

}

void ecc::GameEngine::RenderTiles(SDL_Surface* windowSurface, const SDL_Rect& cameraRect)
{
	Uint32 format = 0;
	int access = 0;
	int width = 0;
	int height = 0;
	SDL_QueryTexture(m_defaultRtv, &format, &access, &width, &height);
	m_tileRtv = SDL_CreateTexture(m_renderer, format, SDL_TEXTUREACCESS_TARGET,
		m_currentMapWidth * TILE_WIDTH * 2,
		m_currentMapHeight * TILE_HEIGHT * 2);

	for (size_t i = 0; i < m_tileSet.size(); ++i) {
		auto image_index_iter = std::find_if(m_tileCoordinateMapping.cbegin(),
			m_tileCoordinateMapping.cend(), [&i](const std::pair<size_t, size_t>& element) {
				return element.second == i;
			});
		auto image_index = image_index_iter->first;

		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		for (const auto& tile : m_tileSet[i]) {
			int res = SDL_RenderCopy(m_renderer, m_images[image_index]->m_texture, &(tile->GetCurrentRect()),
				&(tile->GetDestinationLocation()));
			if (res != 0) {
				std::cerr << SDL_GetError() << "\n";
			}
#ifdef NDEBUG
#else
			if (tile->IsWalkable())
				SDL_RenderDrawRect(m_renderer, &(tile->GetCollisionBox()));
			if (tile->IsBlocked()) {
				SDL_SetRenderDrawColor(m_renderer, 0x00, 0xFF, 0x00, 0xFF);
				SDL_RenderDrawRect(m_renderer, &(tile->GetCollisionBox()));
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
			if (tile->IsClimbable()) {
				SDL_SetRenderDrawColor(m_renderer, 0x00, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawRect(m_renderer, &(tile->GetCollisionBox()));
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			
#endif
		}
	}

}

ecc::Tile* ecc::GameEngine::GetBackgroundTile(const SDL_Rect& location) noexcept
{
	assert(m_tileSet.size() >= 1);

	for (const auto& tile : m_tileSet[0]) {
		if (tile->GetDestinationLocation().x == location.x &&
			tile->GetDestinationLocation().y == location.y) {
			return tile.get();
		}
	}

	return nullptr;
}

ecc::Tile* ecc::GameEngine::GetForegroundTile(const SDL_Rect& location) noexcept
{
	assert(m_tileSet.size() >= 2);

	for (const auto& tile : m_tileSet[1]) {
		if (tile->GetDestinationLocation().x == location.x &&
			tile->GetDestinationLocation().y == location.y) {
			return tile.get();
		}
	}

	return nullptr;
}

bool ecc::GameEngine::CheckMovableBetween(const SDL_Rect& src, const SDL_Rect& target, int increment)
{
	if (src.y != target.y) return false;
	if (src.x == target.x) return true;

	if (src.x < target.x) {
		for (auto i = src.x + increment; i < target.x; i += increment) {
			SDL_Rect tile = { i, src.y, increment * 2, increment * 2 };
			auto tile_ptr = GetBackgroundTile(tile);
			if (!tile_ptr) return false;
			if (tile_ptr->IsLit())
				return false;
		}
	}
	else {
		for (auto i = src.x - increment; i > target.x; i -= increment) {
			SDL_Rect tile = { i, src.y, increment, increment };
			auto tile_ptr = GetBackgroundTile(tile);
			if (!tile_ptr) return false;
			if (tile_ptr->IsLit())
				return false;
		}
	}
	return true;
}

int ecc::GameEngine::GetSingleDirectionMoveBound(const SDL_Rect& nextArea, int increment)
{
	if (GetBackgroundTile(nextArea)->IsLit())
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

void ecc::GameEngine::MoveEnemy(SDL_Surface* windowSurface)
{
	for (auto& enemy : m_enemies) {

#ifdef NDEBUG
#else
		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(m_renderer, &(enemy->GetCollisionBox()));
		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
#endif

		auto collided = std::any_of(m_walkableTiles.begin(), m_walkableTiles.end(),
			[&](const SDL_Rect& rect) {
				return enemy->CheckCollision(rect);
			});

		if (!collided) {
			enemy->Move(24);
		}
		else {
			enemy->Move();
		}

		auto change_direction = std::any_of(m_collisionBoxes.begin(), m_collisionBoxes.end(),
			[&](const SDL_Rect& rect) {
				return enemy->CheckCollision(rect);
			});

		if (change_direction) {
			enemy->FlipDirection();
			for (auto i = 0; i < 10; ++i) {
				enemy->Move();
			}
		}

		auto enemy_pos = enemy->GetCurrentDestination();
		auto daughter_pos = m_daughter->GetCurrentDestination();
		if (enemy_pos.y >= daughter_pos.y) {
			if (enemy_pos.x < daughter_pos.x) {
				enemy->ForceSetDirection(1, SDL_FLIP_NONE);
			}
			else {
				enemy->ForceSetDirection(-1, SDL_FLIP_HORIZONTAL);
			}
		}
		
		enemy->Render(m_renderer);
	}
}

void ecc::GameEngine::GenerateWindows(Scene scene)
{
	switch (scene)
	{
	case ecc::Scene::None:
		break;
	case ecc::Scene::Title:
		break;
	case ecc::Scene::Hallway1:
		break;
	case ecc::Scene::Hallway2:
		break;
	case ecc::Scene::Hallway3:
		break;
	case ecc::Scene::Stage1:
	{
		int x_offset = 2 * 2 * TILE_WIDTH;
		int y_offset = 3 * 2 * TILE_HEIGHT;
		int j = 0;

		m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Closed, 64, 96);
		//m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Open, 272, 96);
		//m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Open, 528, 96);
		//m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Closed, 736, 96);
		//m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Open, 944, 96);
		if (!m_windowGenerated) {
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window3Open, ObjectList::Window3Closed, 288, 96, 2, 2, true, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window3Open, ObjectList::Window3Closed, 544, 96, 2, 2, false, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window3Open, ObjectList::Window3Closed, 736, 96, 2, 2, false, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window3Open, ObjectList::Window3Closed, 960, 96, 2, 2, true, 0));

			/*m_objectFactory->CreateWindow(m_renderer, ObjectList::Window3Open, 64, 416);
			m_objectFactory->CreateWindow(m_renderer, ObjectList::Window1Open, 704, 448);
			m_objectFactory->CreateWindow(m_renderer, ObjectList::Window1Open, 832, 448);
			m_objectFactory->CreateWindow(m_renderer, ObjectList::Window1Open, 960, 448);
			m_objectFactory->CreateWindow(m_renderer, ObjectList::Window1Open, 1088, 448);
			m_objectFactory->CreateWindow(m_renderer, ObjectList::Window1Open, 1216, 448);*/

			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window3Open, ObjectList::Window3Closed, 64, 416, 2, 2, true, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window1Open, ObjectList::Window1Closed, 704, 448, 1, 2, false, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window1Open, ObjectList::Window1Closed, 832, 448, 1, 2, false, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window1Open, ObjectList::Window1Closed, 960, 448, 1, 2, true, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window1Open, ObjectList::Window1Closed, 1088, 448, 1, 2, false, 0));
			m_switchableWindows.emplace_back(std::make_unique<SwitchableWindow>(ObjectList::Window1Open, ObjectList::Window1Closed, 1216, 448, 1, 2, true, 0));

			for (const auto& window : m_switchableWindows) {
				window->SetCollisionBox(m_objectFactory.get());
			}

			m_windowGenerated = true;
		}
		else {
			for (const auto& window : m_switchableWindows) {
				window->Render(m_renderer, m_objectFactory.get());
#ifdef NDEBUG
#else
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawRect(m_renderer, &(window->GetCollisionBox()));
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
#endif
			}
		}

		break;
	}
	case ecc::Scene::Stage2:
		break;
	case ecc::Scene::Stage3:
		break;
	default:
		break;
	}
}

void ecc::GameEngine::GenerateKeyAndDoors()
{
	m_keys.emplace_back(std::make_unique<Key>(m_renderer, "texture/item_shine.png", 32, 582, 0.5));
	m_keys.emplace_back(std::make_unique<Key>(m_renderer, "texture/item_shine.png", 1456, 208, 0.5));
}

void ecc::GameEngine::ClearScene()
{
	m_switchableWindows.clear();
	m_windowGenerated = false;
	m_daughter.reset();
	m_father.reset();
	m_collisionBoxes.clear();
	m_climbableTiles.clear();
	m_blockedTiles.clear();
	m_walkableTiles.clear();
	m_tileSet.clear();
	m_tileCoordinates.clear();
	m_tileCoordinateMapping.clear();
	m_currentMapHeight = 0;
	m_currentMapWidth = 0;
	m_maps.clear();
	m_animatedSprite.reset();
	m_comboWordSprite.reset();
	m_comboSprite.reset();
	m_hpChunk.clear();
	m_hpBar.reset();
	m_images.clear();

}

void ecc::GameEngine::LoadSprite(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier)
{
	m_images.emplace_back(std::make_unique<Image>());
	auto iter = m_images.end();
	--iter;
	iter->get()->LoadSprite(m_renderer, fileName, xPos, yPos, transparency, magnifier);
}

void ecc::GameEngine::LoadAnimatedSprite(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier, int clipX, int clipY)
{
	m_animatedSprite = std::make_unique<Image>();
	m_animatedSprite->LoadAnimatedSprite(m_renderer, fileName, xPos, yPos, transparency,
		magnifier, clipX, clipY);
}

void ecc::GameEngine::PlayAnimatedSprite()
{
	static auto begin_time = std::chrono::high_resolution_clock::now();
	static auto current_frame = 0;
	static auto frame_count = 5;
	auto current_time_ = std::chrono::high_resolution_clock::now();
	auto duration_ = std::chrono::duration<float, std::chrono::seconds::period>(current_time_ - begin_time).count();

	if (duration_ > 0.12f) {
		++current_frame;
		current_frame = current_frame % frame_count;
		m_animatedSprite->SetTexCoord(current_frame * 864, 0);
		begin_time = current_time_;
	}

	m_animatedSprite->Render(m_renderer);
}

ecc::GameEngine::GameEngine(SDL_Window* window, SDL_Surface* windowSurface)
{
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	m_defaultRtv = SDL_GetRenderTarget(m_renderer);
	m_camera = std::make_unique<Camera>(windowSurface);
	m_objectFactory = std::make_unique<ObjectFactory>(m_renderer);
	//GenerateKeyAndDoors();
	m_mixer = std::make_unique<Mixer>();
}

ecc::GameEngine::~GameEngine()
{
	ClearScene();
	m_mixer.reset();
	m_objectFactory.reset();
	m_camera.reset();

	SDL_DestroyTexture(m_tileRtv);
	SDL_DestroyTexture(m_defaultRtv);
	SDL_DestroyRenderer(m_renderer);
	IMG_Quit();
}

void ecc::GameEngine::LoadMap(const std::string& mapName)
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
	m_currentMapWidth = static_cast<int>(number_array[0].size());
	m_currentMapHeight = static_cast<int>(number_array.size());
}

void ecc::GameEngine::LoadTileSet(SDL_Surface* surface, bool loadBackground, bool loadForeground)
{
	size_t index = 0;

	if (loadBackground) {
		index = m_images.size();
		m_images.emplace_back(std::make_unique<Image>());
		auto iter = m_images.end();
		--iter;
		iter->get()->LoadImage(m_renderer, TILESET_FILENAME.backgroundFile, true, true);

		auto background_coordinates = CalculateTileCoordinates(iter->get()->m_width, iter->get()->m_height);
		m_tileCoordinateMapping.insert(
			std::pair<size_t, size_t>(index, m_tileCoordinates.size()));
		m_tileCoordinates.emplace_back(background_coordinates);
		CreateTiles(index, iter->get()->m_width, iter->get()->m_height, surface);
	}

	if (loadForeground) {
		index = m_images.size();
		m_images.emplace_back(std::make_unique<Image>());
		auto iter = m_images.end();
		--iter;
		iter->get()->LoadImage(m_renderer, TILESET_FILENAME.foregroundFile, true, true);

		auto foreground_coordinates = CalculateTileCoordinates(iter->get()->m_width, iter->get()->m_height);
		m_tileCoordinateMapping.insert(
			std::pair<size_t, size_t>(index, m_tileCoordinates.size()));
		m_tileCoordinates.emplace_back(foreground_coordinates);
		CreateTiles(index, iter->get()->m_width, iter->get()->m_height, surface);
	}
	
	GetAllWalkableTiles();
	//GetAllBlockedTiles();
	//GetAllClimbableTiles();
}

void ecc::GameEngine::LoadCharacter(Character::CharacterFlag characterFlag,
	int xPos, int yPos, float speed, ImageIndexFlag initialStatus)
{
	switch (characterFlag)
	{
	case ecc::Character::CharacterFlag::Father:
		m_father = std::make_unique<Character>(m_renderer,
			FATHER_ANIMATIONS.idleAnimation, FATHER_ANIMATIONS.moveAnimation,
			FATHER_ANIMATIONS.attackAnimation, FATHER_ANIMATIONS.jumpAnimation,
			FATHER_ANIMATIONS.fallAnimation, FATHER_ANIMATIONS.climbAnimation,
			characterFlag, xPos, yPos, speed,
			FATHER_PARAMETERS.idleSpeed, FATHER_PARAMETERS.moveSpeed,
			FATHER_PARAMETERS.attackSpeed, FATHER_PARAMETERS.jumpSpeed, 
			FATHER_PARAMETERS.fallSpeed, FATHER_PARAMETERS.climbSpeed, initialStatus);
		break;
	case ecc::Character::CharacterFlag::Daughter:
		m_daughter = std::make_unique<Character>(m_renderer,
			DAUGHTER_ANIMATIONS.idleAnimation, DAUGHTER_ANIMATIONS.moveAnimation,
			DAUGHTER_ANIMATIONS.attackAnimation, DAUGHTER_ANIMATIONS.jumpAnimation,
			DAUGHTER_ANIMATIONS.fallAnimation, DAUGHTER_ANIMATIONS.climbAnimation,
			characterFlag, xPos, yPos, speed,
			DAUGHTER_PARAMETERS.idleSpeed, DAUGHTER_PARAMETERS.moveSpeed,
			DAUGHTER_PARAMETERS.attackSpeed, DAUGHTER_PARAMETERS.jumpSpeed,
			DAUGHTER_PARAMETERS.fallSpeed, DAUGHTER_PARAMETERS.climbSpeed, initialStatus);
		break;
	default:
		break;
	}
}

const ecc::GameStatus& ecc::GameEngine::GetCurrentGameStatus() const noexcept
{
	return m_currentGameStatus;
}

void ecc::GameEngine::CreateCollisionBox(int xPos, int yPos, int width, int height)
{
	auto rect = SDL_Rect();
	rect.x = xPos;
	rect.y = yPos;
	rect.w = width;
	rect.h = height;
	m_collisionBoxes.emplace_back(rect);
}

void ecc::GameEngine::GenerateEnemy()
{
	static auto start_time = std::chrono::high_resolution_clock::now();
	static auto duration_countdown_base = std::chrono::high_resolution_clock::now();
	
	auto current_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
	auto countdown_timer = std::chrono::duration<float, std::chrono::seconds::period>(current_time - duration_countdown_base).count();
	
	static auto lower_bound = 5;
	static auto upper_bound = 8;

	static auto random_seconds = GetRandomInt(lower_bound, upper_bound);
	if (duration > random_seconds) {
		auto random_index = GetRandomInt(0, 5);
		LoadEnemy(RandomEnemyPositions[random_index].x, RandomEnemyPositions[random_index].y, 3.5f);
		m_mixer->PlaySound(SoundList::EnemyRealize, 1);
		random_seconds = GetRandomInt(lower_bound, upper_bound);
		start_time = current_time;
	}
	if (countdown_timer > 15.0f && lower_bound != 0 && upper_bound != 3) {
		--lower_bound;
		--upper_bound;
		duration_countdown_base = current_time;
	}
}

void ecc::GameEngine::LoadHpBar(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier)
{
	m_hpBar = std::make_unique<Image>();
	m_hpBar->LoadSprite(m_renderer, fileName, xPos, yPos, transparency, magnifier);
}

void ecc::GameEngine::LoadHpChunk(const std::string& fileName, int xPos, int yPos, bool transparency, float magnifier)
{
	m_hpChunk.emplace_back(std::make_unique<Image>());
	auto iter = m_hpChunk.end();
	--iter;
	iter->get()->LoadSprite(m_renderer, fileName, xPos, yPos, transparency, magnifier);
}

void ecc::GameEngine::LoadComboSprite(const std::string& numberFileName,
	const std::string& comboFileName,
	int xPos, int yPos, bool transparency, float magnifier, int clipX, int clipY)
{
	m_comboSprite = std::make_unique<Image>();
	m_comboSprite->LoadAnimatedSprite(m_renderer, numberFileName, xPos, yPos, transparency,
		magnifier, clipX, clipY);
	m_comboWordSprite = std::make_unique<Image>();
	m_comboWordSprite->LoadSprite(m_renderer, comboFileName, xPos + 128, yPos + 64, transparency);
}

void ecc::GameEngine::SwitchComboSprite()
{
	if (m_combo < 3) return;
	auto offset = m_combo * 64;
	m_comboSprite->SetTexCoord(offset, 0);
	m_comboSprite->Render(m_renderer);
	m_comboWordSprite->Render(m_renderer);
}

void ecc::GameEngine::LoadEnemy(int xPos, int yPos, float speed)
{
	m_enemies.emplace_back(std::make_unique<Enemy>(m_renderer,
		ENEMY_ANIMATIONS.idleAnimation, ENEMY_ANIMATIONS.moveAnimation,
		ENEMY_ANIMATIONS.attackAnimation,
		xPos, yPos, speed,
		ENEMY_PARAMETERS.idleSpeed, ENEMY_PARAMETERS.moveSpeed, ENEMY_PARAMETERS.attackSpeed));
}

void ecc::GameEngine::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	SDL_RenderClear(m_renderer);
}

void ecc::GameEngine::Render(SDL_Surface* windowSurface, Scene scene)
{
	
	for (const auto& image : m_images) {
		if (image->m_isTile) continue;

		image->Render(m_renderer);
	}

	if (scene == Scene::GameOver) {
		SDL_RenderPresent(m_renderer);
		return;
	}

	if (scene == Scene::Defense) {
		PlayAnimatedSprite();

		RenderTiles(windowSurface, { 0, 0, 0, 0 });

#ifdef NDEBUG
#else
		for (const auto& box : m_collisionBoxes) {
			SDL_SetRenderDrawColor(m_renderer, 0x66, 0x66, 0x66, 0xFF);
			SDL_RenderDrawRect(m_renderer, &box);
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
#endif

		UpdateCharacters(0, 0, windowSurface);

		GenerateEnemy();
		MoveEnemy(windowSurface);

#ifdef NDEBUG
#else
		if (m_father && m_daughter) {
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(m_renderer, &(m_father->GetAttackCollisionBox()));
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawRect(m_renderer, &(m_daughter->GetCollisionBox()));
		}
#endif

		if (m_hpBar)
			m_hpBar->Render(m_renderer);

		if (!m_hpChunk.empty()) {
			float percentage = static_cast<float>(m_hp) / 500.0f;
			float show_percentage = percentage * 149.0f;
			for (auto i = 0; i < static_cast<size_t>(show_percentage); ++i) {
				m_hpChunk[i]->Render(m_renderer);
			}
		}

		if (m_comboSprite)
			SwitchComboSprite();
	}

	//for (const auto& object : m_objects) {
	//	object->Render(m_renderer);
	//}*/

	/*SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(m_renderer, m_tileRtv, nullptr, nullptr);*/

	//GenerateWindows(scene);

	/*for (const auto& key : m_keys) {
		key->Render(m_renderer, 1.0f);
	}*/

	SDL_RenderPresent(m_renderer);

	auto is_daughter_hit = std::any_of(m_enemies.cbegin(), m_enemies.cend(), [&](const std::unique_ptr<Enemy>& enemy) {
		return m_daughter->CheckCollision(enemy->GetCollisionBox());
		});

	if (is_daughter_hit) {
		m_currentGameStatus = GameStatus::GameOver;
		m_mixer->PlaySound(SoundList::Scream, 1);
		return;
	}

	auto iter = m_enemies.begin();
	while (iter != m_enemies.end()) {
		if (m_father->CheckCollision(iter->get()->GetCollisionBox())) {
			m_hp -= 50;
			iter = m_enemies.erase(iter);
			m_combo = 0;
			break;
		}
		else {
			++iter;
		}
	}
}

void ecc::GameEngine::SwitchLight()
{
	for (const auto& window : m_switchableWindows) {
		window->Switch(0);
		int x_range = 0;
		int y_range = 0;
		window->GetLightingRange(x_range, y_range);
		auto location = window->GetCurrentDestination(m_objectFactory.get());

		auto tile_1 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h,
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_2 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h + (TILE_HEIGHT * 2),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_3 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h,
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_4 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h + (TILE_HEIGHT * 2),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_5 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h + (TILE_HEIGHT * 4),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_6 = GetBackgroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h + (TILE_HEIGHT * 4),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});


		auto tile_7 = GetForegroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h,
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_8 = GetForegroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h + (TILE_HEIGHT * 2),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_9 = GetForegroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h,
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_10 = GetForegroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h + (TILE_HEIGHT * 2),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_11 = GetForegroundTile({
			location.x + (TILE_WIDTH * 2), location.y + location.h + (TILE_HEIGHT * 4),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});
		auto tile_12 = GetForegroundTile({
			location.x + (TILE_WIDTH * 4), location.y + location.h + (TILE_HEIGHT * 4),
			TILE_WIDTH * 2, TILE_HEIGHT * 2
			});

		tile_1->ChangeStatus(!tile_1->IsLit());
		tile_2->ChangeStatus(!tile_2->IsLit());
		tile_3->ChangeStatus(!tile_3->IsLit());
		tile_4->ChangeStatus(!tile_4->IsLit());
		tile_5->ChangeStatus(!tile_5->IsLit());
		tile_6->ChangeStatus(!tile_6->IsLit());
		tile_7->ChangeStatus(!tile_7->IsLit());
		tile_8->ChangeStatus(!tile_8->IsLit());
		tile_9->ChangeStatus(!tile_9->IsLit());
		tile_10->ChangeStatus(!tile_10->IsLit());
		tile_11->ChangeStatus(!tile_11->IsLit());
		tile_12->ChangeStatus(!tile_12->IsLit());
	}

}

void ecc::GameEngine::SetCharacterIndex()
{
	if (m_characterIndex == 0) {
		m_characterIndex = 1;
		m_father->SetStatus(CharacterStatusFlag::Incontrollable);
		m_daughter->SetStatus(CharacterStatusFlag::Controllable);
	}
	else if (m_characterIndex == 1) {
		m_characterIndex = 0;
		m_father->SetStatus(CharacterStatusFlag::Controllable);
		m_daughter->SetStatus(CharacterStatusFlag::Incontrollable);
	}
}

unsigned short ecc::GameEngine::GetCharacterIndex() const noexcept
{
	return m_characterIndex;
}