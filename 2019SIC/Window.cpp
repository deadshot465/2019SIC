#include "Window.h"

ecc::GameWindow::GameWindow(const std::string& title, int width, int height,
	bool fullScreen, int xPos, int yPos)
{
	SDL_Init(SDL_INIT_VIDEO);

	Uint32 flags = SDL_WINDOW_SHOWN;

	if (fullScreen) {
		auto displays = SDL_GetNumVideoDisplays();
		SDL_DisplayMode display_mode = {};
		SDL_GetCurrentDisplayMode(displays - 1, &display_mode);
		SDL_Rect bounds = {};
		SDL_GetDisplayBounds(displays - 1, &bounds);
		width = bounds.w;
		height = bounds.h;

		m_window = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		m_window = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);
	}

	m_scaleX = static_cast<float>(width) / MAP_WIDTH;
	m_scaleY = static_cast<float>(height) / MAP_HEIGHT;

	m_surface = SDL_GetWindowSurface(m_window);

	m_graphicsEngine = std::make_unique<GameEngine>(m_window, m_surface);
	m_graphicsEngine->LoadMap("map/Stage2_background.txt", "texture/Background_ver.2 (copy).png", m_surface);
	m_graphicsEngine->LoadMap("map/Stage2_foreground.txt", "texture/Foreground_ver.2.0.png", m_surface);

	m_graphicsEngine->LoadObject("texture/door_new.png",
		MAX_MAP_X * TILE_WIDTH - (5 * TILE_WIDTH),
		MAX_MAP_Y * TILE_HEIGHT - (5 * TILE_HEIGHT));

	m_graphicsEngine->LoadEnemy("texture/vampire_enemy_idle.png",
		"texture/vampire_enemy_walk.png",
		"texture/vampire_enemy_run.png",
		25 * (TILE_WIDTH * 2),
		height - (TILE_HEIGHT * 2) - (CHARACTER_SPRITE_HEIGHT * 2), 2.5f, 3);

	m_graphicsEngine->LoadCharacter("texture/vampire_idle.png",
		"texture/vampire_run.png",
		"texture/vampire_slash.png",
		"texture/vampire_jump(rise).png",
		"texture/vampire_jump(fall).png",
		ecc::Character::CharacterFlag::Father,
		15 * (TILE_WIDTH * 2),
		height - (TILE_HEIGHT * 2) - (CHARACTER_SPRITE_HEIGHT * 2), 15.0f);

	m_graphicsEngine->LoadCharacter("texture/girl_idle.png",
		"texture/girl_walk.png",
		"texture/vampire_slash.png",
		"texture/girl_jump(rise).png",
		"texture/girl_jump(fall).png",
		ecc::Character::CharacterFlag::Daughter,
		5 * (TILE_WIDTH * 2),
		height - (TILE_HEIGHT * 2) - (CHARACTER_SPRITE_HEIGHT * 2), 10.0f);

	SDL_UpdateWindowSurface(m_window);
	m_isInit = true;
}

ecc::GameWindow::~GameWindow()
{
	SDL_FreeSurface(m_surface);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void ecc::GameWindow::Broadcast()
{
	while (SDL_PollEvent(&m_event)) {
		switch (m_event.type) {
		case SDL_EventType::SDL_QUIT:
			m_isInit = false;
			break;
		case SDL_EventType::SDL_KEYDOWN:
		{
			switch (m_event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_isInit = false;
				break;
			case SDLK_l:
				m_graphicsEngine->SwitchLight();
				break;
			case SDLK_LSHIFT:
			{
				m_graphicsEngine->SetCharacterIndex();
				break;
			}
			default:
				break;
			}
		}
		}
	}

	m_graphicsEngine->Clear(0xFF, 0xFF, 0xFF, 0xFF);
	m_graphicsEngine->Render(m_surface, m_scaleX, m_scaleY);
}

bool ecc::GameWindow::IsInit() const noexcept
{
	return m_isInit;
}