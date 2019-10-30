#include "AudioPlayer.h"
#include "../Config.h"
#include <windows.h>

void AudioPlayer::playSound(const std::string& file)
{
	const std::string sfxPath = Config::get().get("_engine", "sfxPath");
	PlaySound((sfxPath + "/" + file).c_str(), nullptr, SND_FILENAME | SND_ASYNC);
}

void AudioPlayer::setActiveMusic(const std::string& file)
{
	stopMusic();

	const std::string musicPath = Config::get().get("_engine", "musicPath");
	mciSendString(
		("open \"" + musicPath + "/" + file + "\" type mpegvideo alias musix").c_str(), 
		nullptr, 0, 0);
	mciSendString("play musix repeat", nullptr, 0, 0);
}

void AudioPlayer::stopMusic()
{
	mciSendString("stop musix", nullptr, 0, 0);
	mciSendString("close musix", nullptr, 0, 0);
}
