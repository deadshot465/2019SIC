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
		m_scaleX = static_cast<float>(width) / (TILE_WIDTH * MAX_MAP_X);
		m_scaleY = static_cast<float>(height) / (TILE_HEIGHT * MAX_MAP_Y);

		m_window = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		m_window = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);
	}
	
	m_surface = SDL_GetWindowSurface(m_window);

	m_graphicsEngine = std::make_unique<GraphicsEngine>(m_window);
	m_graphicsEngine->LoadMap("map/SIC_Demo_revised.txt");

	m_graphicsEngine->LoadImage("texture/SIC_demo_tiles.png", true);
	m_graphicsEngine->LoadCharacter("texture/girl_walk.png",
		5 * TILE_WIDTH,
		MAX_MAP_Y * TILE_HEIGHT - TILE_HEIGHT - CHARACTER_SPRITE_HEIGHT);

	m_graphicsEngine->LoadCharacter("texture/vampire_run.png",
		10 * TILE_WIDTH,
		MAX_MAP_Y * TILE_HEIGHT - TILE_HEIGHT - CHARACTER_SPRITE_HEIGHT);

	m_isInit = true;
}

ecc::GameWindow::~GameWindow()
{
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
			default:
				break;
			}
		}
		}

	}

	m_graphicsEngine->Clear(0xFF, 0xFF, 0xFF, 0xFF, m_scaleX, m_scaleY);
	m_graphicsEngine->Render();
}

bool ecc::GameWindow::IsInit() const noexcept
{
	return m_isInit;
}
