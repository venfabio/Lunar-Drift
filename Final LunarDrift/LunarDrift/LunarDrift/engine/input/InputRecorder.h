#pragma once

#include <queue>
#include <ctime>

/**
 * @brief Input recorder; used for producing replays via input injection
 *
 * This class can be plugged into the InputMap to record input at given timestamps.
 * When desired, the recorded inputs can be played back, whereupon this class
 * injects inputs into the InputMap when their times are due, thus permitting
 * simple replays.
 *
 * @author Hayley Hatton
 * @date   27/05/2016
 * @see    InputMap
 * @see    Keyboard
 * @ingroup Inputs
 */
class InputRecorder
{
public:
    /** @brief Default constructor */
	InputRecorder();

    /** @brief Default destructor */
	~InputRecorder();


	void startRecording();

	void stopRecording();

	/**
	 * @brief Set a key into a down/pressed state
	 * @param key Key identifier
	 */
	void onKeyDown(int key);

	/**
	 * @brief Set a key into an up/released state
	 * @param key Key identifier
	 */
	void onKeyUp(int key);

	void stepReplay(double dt);

	bool completedReplay() const { return m_RewindTime > m_Duration; }

private:
	std::queue<std::pair<double, std::pair<int, bool>>> m_InputQueue;
	clock_t m_StartTime, m_PausedTime;
	double m_RewindTime, m_Duration, m_PausedDuration;
	bool m_Paused, m_Replaying;
};
