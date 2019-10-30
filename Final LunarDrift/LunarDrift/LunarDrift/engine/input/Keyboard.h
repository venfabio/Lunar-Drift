#pragma once

#include "InputRecorder.h"
#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include <memory>

typedef uint16_t Key;

/**
 * @brief Keyboard input representation object
 *
 * This class manages abstract keyboard state whilst providing a mapping
 * between hardware input state and abstract engine "button-type" input classes.
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    InputMap
 * @ingroup Inputs
 */
class Keyboard
{
public:
    /** @brief Default constructor */
    Keyboard();

    /** @brief Default destructor */
    virtual ~Keyboard();


	/**
	* @brief Returns the key associated with a given input class name
	* @param inputName Name of the input to check
	* @return Key identifier
	*/
	Key getKeyAssociatedWithMap(const std::string& inputName) const;

    /**
     * @brief Maps a given key to a "button-type" input class name
     * @param key Key identifier
     * @param inputName Name of the input
     */
    void mapKeyToInput(Key key, const std::string& inputName);

    /**
     * @brief Unmaps a given key from an input class name
     * @param key Key identifier
     */
    void unmapKeyFromInputs(Key key);

    /**
     * @brief Query if a given key associated with an input class name is down
     * @param inputName Name of the input
     * @return True if pressed/down; false if released/up
     */
    bool isKeyDown(const std::string& inputName);

    /**
     * @brief Set a key into a down/pressed state
     * @param key Key identifier
     */
    void setKeyDown(Key key);

    /**
     * @brief Set a key into an up/released state
     * @param key Key identifier
     */
    void setKeyUp(Key key);

	/**
	* @brief Set an input recorder to record inputs
	* @param recorder Managed ptr to recorder instance
	*/
	void setInputRecorder(std::shared_ptr<InputRecorder> recorder) { m_Recorder = recorder; }

private:
    std::vector<bool> m_KeyStates;  //!< Record of current key states
    std::map<Key,std::string> m_KeyInputMap;    //!< Record of key-input mapping
	std::shared_ptr<InputRecorder> m_Recorder;	//!< Recorder for key-input re-injection
};
