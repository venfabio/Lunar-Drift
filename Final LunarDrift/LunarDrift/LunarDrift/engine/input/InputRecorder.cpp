#include "InputRecorder.h"
#include "InputMap.h"

InputRecorder::InputRecorder()
	: m_StartTime(0), 
	  m_PausedTime(0),
	  m_RewindTime(0.0), 
	  m_Duration(1.0),
	  m_PausedDuration(0.0),
	  m_Paused(false), 
	  m_Replaying(false)
{

}

InputRecorder::~InputRecorder()
{

}


void InputRecorder::startRecording()
{
	m_StartTime = clock();
	m_Replaying = false;
	m_RewindTime = 0.0;
	m_Duration = 0.0;
}

void InputRecorder::stopRecording()
{
	clock_t now = clock();
	m_Replaying = true;
	m_RewindTime = -3.0;
	m_Duration = (static_cast<double>(now - m_StartTime) / CLOCKS_PER_SEC) - m_PausedDuration;
}

void InputRecorder::onKeyDown(int key)
{
	Keyboard* kbd = InputMap::getInstance().getKeyboard();
	if (key == kbd->getKeyAssociatedWithMap("PreemptiveEnd"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("BirdsEye"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("BirdsEye2"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("DebugCollisions"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("Pause"))
	{
		if (m_Paused)
		{
			if (!m_Replaying)
			{
				clock_t now = clock();
				m_PausedDuration += static_cast<double>(now - m_PausedTime) / CLOCKS_PER_SEC;
			}

			m_Paused = false;
		}
		else 
		{
			if (!m_Replaying)
				m_PausedTime = clock();
			m_Paused = false;
		}
	}
	else
	{
		if (!m_Paused && !m_Replaying)
		{
			clock_t now = clock();
			m_InputQueue.push(
				{ (static_cast<double>(now - m_StartTime) / CLOCKS_PER_SEC) - m_PausedDuration, 
				{ key, true } });
		}
	}
}

void InputRecorder::onKeyUp(int key)
{
	Keyboard* kbd = InputMap::getInstance().getKeyboard();
	if (key == kbd->getKeyAssociatedWithMap("PreemptiveEnd"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("DebugCollisions"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("BirdsEye"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("BirdsEye2"))
		return;
	else if (key == kbd->getKeyAssociatedWithMap("Pause"))
		return;

	if (!m_Paused && !m_Replaying)
	{
		clock_t now = clock();
		m_InputQueue.push(
			{ (static_cast<double>(now - m_StartTime) / CLOCKS_PER_SEC) - m_PausedDuration,
			{ key, false } });
	}
}

void InputRecorder::stepReplay(double dt)
{
	if (!m_Paused)
	{
		Keyboard* kbd = InputMap::getInstance().getKeyboard();

		m_RewindTime += dt;
		bool tasksToDo = !m_InputQueue.empty();
		while (tasksToDo)
		{
			if (m_InputQueue.front().first <= m_RewindTime)
			{
				std::pair<double, std::pair<int, bool>> record = m_InputQueue.front();
				m_InputQueue.pop();

				if (record.second.second)
					kbd->setKeyDown(record.second.first);
				else
					kbd->setKeyUp(record.second.first);
			}
			else
				tasksToDo = false;

			if (m_InputQueue.empty())
				tasksToDo = false;
		}
	}
}
