#pragma once

#include <cstdlib>
#include <memory>
#include "../Program.h"

/** @brief Encapsulation of C return-to-desktop signals */
enum class ExitStatus
{
    Success = EXIT_SUCCESS,
    Failure = EXIT_FAILURE
};

/**
 * @brief Abstract window container, for bridging the system to the program
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup TopLevelContainers
 */
class Window
{
public:
    /** @brief Default constructor */
    Window();

    /** @brief Default destructor */
    virtual ~Window();

    /**
     * @brief Main execution point for the window
     * @param program Pointer to program to execute
     * @return State indicator to inform the desktop whether execution succeeded
     */
	virtual ExitStatus run(std::unique_ptr<Program> program) = 0;

protected:
    /**
     * @brief Access the currently-executing program instance
     * @return Pointer to the currently-executing program; nullptr if none
     */
	Program* getProgram() { return m_Program; }

	std::string getGPUName() const;

private:
    Program* m_Program;     //!< Pointer to program class
};
