#include "GraphicsEngine.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <SDL_image.h>

void ecc::GraphicsEngine::CalculateTileCoordinates()
{
	int x = 0;
	int y = 0;
	for (auto& row : m_tileCoordinates) {
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
}

ecc::GraphicsEngine::GraphicsEngine(SDL_Window* window)
{
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

ecc::GraphicsEngine::~GraphicsEngine()
{
	SDL_DestroyRenderer(m_renderer);
	IMG_Quit();
}

void ecc::GraphicsEngine::LoadImage(const std::string& fileName)
{
	m_images.emplace_back(std::make_unique<Image>());
	auto iter = m_images.end();
	--iter;
	iter->get()->LoadImage(m_renderer, fileName);

	CalculateTileCoordinates();
}

void ecc::GraphicsEngine::LoadMap(const std::string& mapName)
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
				long location = fs.tellg();
				fs.seekg(location + 1);
			}
			else {
				m_map.emplace_back(temp);
			}
		} while (!fs.fail());
			
	}

	fs.close();
}

void ecc::GraphicsEngine::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	SDL_RenderClear(m_renderer);
}

void ecc::GraphicsEngine::Render()
{
	for (const auto& image : m_images) {

		auto src_rect = SDL_Rect();
		auto dst_rect = SDL_Rect();

		for (auto i = 0; i < MAX_MAP_Y; ++i) {
			for (auto j = 0; j < MAX_MAP_X; ++j) {
				auto tile = m_map[i * MAX_MAP_X + j];
				if (tile < MAX_TILE_COUNT_X) {
					src_rect = m_tileCoordinates[0][tile];
				}
				else {
					src_rect = m_tileCoordinates[1][tile - MAX_TILE_COUNT_X];
				}
				dst_rect.x = j * TILE_WIDTH;
				dst_rect.y = i * TILE_HEIGHT;
				dst_rect.w = TILE_WIDTH;
				dst_rect.h = TILE_HEIGHT;
				SDL_RenderCopy(m_renderer, image->GetTexture(),
					&src_rect, &dst_rect);
			}
		}
	}

	SDL_RenderPresent(m_renderer);
}