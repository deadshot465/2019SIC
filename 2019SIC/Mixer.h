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
		inline static const std::string SFX_RUNNING = "WAV/Running.wav";
		inline static const std::string SFX_WALKING = "WAV/Walking.wav";
		inline static const std::string SFX_PUNCH = "WAV/Punch.wav";
		inline static const std::string SFX_PUNCH_2 = "WAV/Dad_swing_hit.wav";
		inline static const std::string SFX_PUNCH_3 = "WAV/Middle_punch.wav";
		inline static const std::string SFX_SWISH = "WAV/Swish.wav";
		inline static const std::string SFX_STRIKE = "WAV/Striking.wav";
		inline static const std::string SFX_ENEMY_WAITING = "WAV/Enemy_growl.wav";
		inline static const std::string SFX_ENEMY_REALIZE = "WAV/Enemy_found_player.wav";
		inline static const std::string SFX_LEVER = "WAV/Lever_switch.wav";
		inline static const std::string SFX_CURTAIN = "WAV/Curtain.wav";
		inline static const std::string SFX_CURTAIN_2 = "WAV/Curtain_2.wav";
		inline static const std::string SFX_KEY_PICK = "WAV/Key_pickup.wav";
		inline static const std::string SFX_KEY_UNLOCKED = "WAV/Open_key.wav";
		inline static const std::string SFX_DOOR_OPEN = "WAV/Door_open.wav";
		inline static const std::string SFX_BREAK_DOOR = "WAV/Break.wav";
		inline static const std::string SFX_LADDER_STEP = "WAV/Ladder_step.wav";
		inline static const std::string SFX_STAIRS = "WAV/Stairs.wav";
		inline static const std::string SFX_MOVE_WOODBOX = "WAV/Woodbox.wav";

		std::unordered_map<SoundList, Mix_Music*> m_soundMapping;

	public:
		Mixer();
		~Mixer();

		void PlaySound(SoundList sound, int loops = 0);
	};
};

