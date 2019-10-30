#pragma once

#pragma comment (lib, "winmm.lib")

#include <string>

/**
 * @defgroup Audio
 * @brief Classes pertaining to the management of audio in the game
 */

/**
 * @brief Singletonized audio player for playing sounds and music
 *
 * @author Hayley Hatton
 * @date   26/05/2016
 * @ingroup Audio
 */
class AudioPlayer
{
public:
	/**
	 * @brief Singleton access
	 */
	static AudioPlayer& getInstance()
	{
		static AudioPlayer instance;
		return instance;
	}

	/**
	 * @brief Play a brief sound effect that only plays once
	 * @brief file Filename of the sound-effect
	 */
	void playSound(const std::string& file);

	/**
	 * @brief Set music to begin playing; will loop around
	 * @brief file Filename of the song
	 */
	void setActiveMusic(const std::string& file);

	/**
	 * @brief Stop playing the currently-active song, if any
	 */
	void stopMusic();


private:
	// Singletonize
	AudioPlayer() {};
	AudioPlayer(AudioPlayer const&) = delete;
	void operator=(AudioPlayer const&) = delete;
};
