#include "SceneManager.h"
#include "Character.h"

ecc::SceneManager::SceneManager(SDL_Window* window, SDL_Surface* surface)
{
	m_gameEngine = std::make_unique<GameEngine>(window, surface);
}

ecc::SceneManager::~SceneManager()
{
	m_gameEngine.reset();
}

void ecc::SceneManager::LoadScene(SDL_Window* window, SDL_Surface* surface, Scene scene)
{
	switch (scene)
	{
	case ecc::Scene::None:
		break;
	case ecc::Scene::Title:
		m_gameEngine->ClearScene();
		m_gameEngine->LoadSprite(TITLE_SCREEN_FILENAME, 0, 0, true, 0.75f);
		m_currentScene = Scene::Title;
		break;
	case ecc::Scene::Hallway1:
		m_gameEngine->ClearScene();
		SDL_SetWindowSize(window, 1728, 640);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		m_gameEngine->LoadMap(STAGE_HALL1_FILENAME.backgroundFile);
		m_gameEngine->LoadMap(STAGE_HALL1_FILENAME.foregroundFile);
		m_gameEngine->LoadTileSet(surface);
		m_gameEngine->LoadCharacter(Character::CharacterFlag::Father,
			16 * (TILE_WIDTH * 2), 2 * (TILE_HEIGHT * 2), 16.0f);
		m_gameEngine->LoadCharacter(Character::CharacterFlag::Daughter,
			18 * (TILE_WIDTH * 2), 2 * (TILE_HEIGHT * 2), 8.0f);
		m_currentScene = Scene::Hallway1;
		break;
	case ecc::Scene::Hallway2:
		m_gameEngine->ClearScene();
		m_gameEngine->LoadMap(STAGE_HALL2_FILENAME.backgroundFile);
		m_gameEngine->LoadMap(STAGE_HALL2_FILENAME.foregroundFile);
		m_gameEngine->LoadTileSet(surface);
		m_currentScene = Scene::Hallway2;
		break;
	case ecc::Scene::Hallway3:
		m_gameEngine->ClearScene();
		m_gameEngine->LoadMap(STAGE_HALL3_FILENAME.backgroundFile);
		m_gameEngine->LoadMap(STAGE_HALL3_FILENAME.foregroundFile);
		m_gameEngine->LoadTileSet(surface);
		m_currentScene = Scene::Hallway3;
		break;
	case ecc::Scene::Stage1:
		m_gameEngine->ClearScene();
		m_gameEngine->LoadMap(STAGE_1_FILENAME.backgroundFile);
		m_gameEngine->LoadMap(STAGE_1_FILENAME.foregroundFile);
		m_gameEngine->LoadTileSet(surface);
		m_gameEngine->LoadCharacter(Character::CharacterFlag::Father,
			15 * (TILE_WIDTH * 2), 15 * (TILE_HEIGHT * 2), 16.0f);
		m_gameEngine->LoadCharacter(Character::CharacterFlag::Daughter,
			5 * (TILE_WIDTH * 2), 15 * (TILE_HEIGHT * 2), 8.0f);
		m_currentScene = Scene::Stage1;
		break;
	case ecc::Scene::Stage2:
		break;
	case ecc::Scene::Stage3:
		break;
	case ecc::Scene::GameOver:
		m_gameEngine->ClearScene();
		SDL_SetWindowSize(window, 1152, 810);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		m_gameEngine->LoadSprite(GAMEOVER_SCREEN_FILENAME, 0, 0, true, 0.75f);
		m_currentScene = Scene::GameOver;
		break;
	default:
		break;
	}
}

void ecc::SceneManager::Render(SDL_Surface* surface, float scaleX, float scaleY)
{
	m_gameEngine->Clear(0xFF, 0xFF, 0xFF, 0xFF);
	m_gameEngine->Render(surface, m_currentScene);

}

const ecc::Scene& ecc::SceneManager::GetCurrentScene() const noexcept
{
	return m_currentScene;
}

const ecc::GameStatus& ecc::SceneManager::GetCurrentGameStatus() const noexcept
{
	return m_gameEngine->GetCurrentGameStatus();
}
