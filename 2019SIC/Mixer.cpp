#include "Mixer.h"

ecc::Mixer::Mixer()
{
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD);
	Mix_OpenAudio(22050, AUDIO_S16, 2, 4096);

	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Running, Mix_LoadMUS(SFX_RUNNING.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Walking, Mix_LoadMUS(SFX_WALKING.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Punch1, Mix_LoadMUS(SFX_PUNCH.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Punch2, Mix_LoadMUS(SFX_PUNCH_2.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Punch3, Mix_LoadMUS(SFX_PUNCH_3.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Swish, Mix_LoadMUS(SFX_SWISH.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Strike, Mix_LoadMUS(SFX_STRIKE.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::EnemyWating, Mix_LoadMUS(SFX_ENEMY_WAITING.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::EnemyRealize, Mix_LoadMUS(SFX_ENEMY_REALIZE.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Lever, Mix_LoadMUS(SFX_LEVER.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Curtain1, Mix_LoadMUS(SFX_CURTAIN.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Curtain2, Mix_LoadMUS(SFX_CURTAIN_2.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::KeyPickUp, Mix_LoadMUS(SFX_KEY_PICK.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::KeyUnlocked, Mix_LoadMUS(SFX_KEY_UNLOCKED.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::DoorOpen, Mix_LoadMUS(SFX_DOOR_OPEN.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::BreakingDoor, Mix_LoadMUS(SFX_BREAK_DOOR.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::LadderStep, Mix_LoadMUS(SFX_LADDER_STEP.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Stairs, Mix_LoadMUS(SFX_STAIRS.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::MovingBox, Mix_LoadMUS(SFX_MOVE_WOODBOX.c_str())));
	m_soundMapping.insert(std::pair<SoundList, Mix_Music*>
		(SoundList::Scream, Mix_LoadMUS(SFX_SCREAM.c_str())));
}

ecc::Mixer::~Mixer()
{
	for (auto& pair : m_soundMapping) {
		Mix_FreeMusic(pair.second);
	}

	Mix_CloseAudio();
	Mix_Quit();
}

void ecc::Mixer::PlaySound(SoundList sound, int loops)
{
	Mix_PlayMusic(m_soundMapping[sound], loops);
}
