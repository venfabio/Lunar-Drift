#pragma once

/** @brief Object-oriented enumeration of race modes */
enum class RaceModeSelection
{
	SinglePlayer, 
	Multiplayer
};

/** @brief Object-oriented enumeration of ship types */
enum class ShipSelection
{
	Derpy,
	PinkiePie
};

/** @brief Object-oriented enumeration of tracks */
enum class TrackSelection
{
	Antares,
	Andromeda,
	KesselRun
};

/** @brief Object-oriented enumeration of difficulty modes */
enum class DifficultySelection
{
	Easy,
	Expert
};

/** @brief Parameters for setting up a RaceScene */
struct GameParameters
{
	RaceModeSelection raceMode;
	ShipSelection ship1, ship2;
	TrackSelection track;
	DifficultySelection difficulty;
};
