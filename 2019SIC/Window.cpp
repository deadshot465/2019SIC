#include "Window.h"

ecc::GameWindow::GameWindow(const std::string& title, int width, int height,
	bool fullScreen, int xPos, int yPos)
{
	SDL_Init(SDL_INIT_VIDEO);

	Uint32 flags = SDL_WINDOW_SHOWN;
	if (fullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	m_window = SDL_CreateWindow(title.c_str(), xPos, yPos, width, height, flags);
	m_surface = SDL_GetWindowSurface(m_window);

	m_graphicsEngine = std::make_unique<GraphicsEngine>(m_window);
	m_graphicsEngine->LoadMap("map/SIC_Demo_revised.txt");

	m_graphicsEngine->LoadImage("texture/SIC_demo_tiles.png", true);

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

		m_graphicsEngine->Clear(0xFF, 0xFF, 0xFF, 0xFF);
		m_graphicsEngine->Render();
	}
}

bool ecc::GameWindow::IsInit() const noexcept
{
	return m_isInit;
}
