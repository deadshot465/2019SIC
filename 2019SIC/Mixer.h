#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include "Helper.h"

namespace ecc {
	class Mixer
	{
	private:
		inline static const std::string SFX_RUNNING = "wav/Running.wav";
		inline static const std::string SFX_WALKING = "wav/Walking.wav";
		inline static const std::string SFX_PUNCH = "wav/Punch.wav";
		inline static const std::string SFX_PUNCH_2 = "wav/Dad_swing_hit.wav";
		inline static const std::string SFX_PUNCH_3 = "wav/Middle_punch.wav";
		inline static const std::string SFX_SWISH = "wav/Swish.wav";
		inline static const std::string SFX_STRIKE = "wav/Striking.wav";
		inline static const std::string SFX_ENEMY_WAITING = "wav/Enemy_growl.wav";
		inline static const std::string SFX_ENEMY_REALIZE = "wav/Enemy_found_player.wav";
		inline static const std::string SFX_LEVER = "wav/Lever_switch.wav";
		inline static const std::string SFX_CURTAIN = "wav/Curtain.wav";
		inline static const std::string SFX_CURTAIN_2 = "wav/Curtain_2.wav";
		inline static const std::string SFX_KEY_PICK = "wav/Key_pickup.wav";
		inline static const std::string SFX_KEY_UNLOCKED = "wav/Open_key.wav";
		inline static const std::string SFX_DOOR_OPEN = "wav/Door_open.wav";
		inline static const std::string SFX_BREAK_DOOR = "wav/Break.wav";
		inline static const std::string SFX_LADDER_STEP = "wav/Ladder_step.wav";
		inline static const std::string SFX_STAIRS = "wav/Stairs.wav";
		inline static const std::string SFX_MOVE_WOODBOX = "wav/Woodbox.wav";
		inline static const std::string SFX_SCREAM = "wav/132106__sironboy__woman-scream.wav";

		std::unordered_map<SoundList, Mix_Music*> m_soundMapping;

	public:
		Mixer();
		~Mixer();

		void PlaySound(SoundList sound, int loops = 0);
	};
};

