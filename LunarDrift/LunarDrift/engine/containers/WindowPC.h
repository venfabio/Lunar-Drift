#pragma once

#include "Window.h"
#include "../graphics/Context.h"
#include "../comms/IMessageListener.h"
#include <Windows.h>
#include <queue>
#include <atomic>
#include <mutex>

/**
 * @brief Window class for use on Windows desktop PCs
 *
 * @pre    Win32 API support must be available to the compiler and running environments
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup TopLevelContainers
 */
class WindowPC : public Window, public IMessageListener
{
public:
    /** @brief Default constructor; sets up the environment */
	WindowPC(
		const std::string& title,
		HINSTANCE hInstance,
		LPSTR lpCmdLine,
		int nCmdShow,
		unsigned int width,
		unsigned int height);

    /** @brief Default destructor */
    virtual ~WindowPC();


    /**
     * @brief Access the underlying graphics context
     * @return Pointer to the underlying graphics context object
     */
    Context* getContext() { return &m_Context; }

    /**
     * @brief Running environment, with the final return status
     * @param program Pointer to the program to run in the window
     * @return Status to exit application and return to desktop
     */
	ExitStatus run(std::unique_ptr<Program> program) override;

	/**
	* @brief Called when a subscribed event is raised
	* @param event Smart pointer to the raised event object
	*/
	void onMessage(std::shared_ptr<Message> event) override;

    /** 
     * @brief Simulation thread function 
     * @param program Pointer to program to step simulation step of
     */
    void simulationThread(Program* program);

    /**
     * @brief Rendering thread function
     * @param context Graphical context
     * @param program Pointer to program to render with
     */
	void renderingThread(Context* context, Program* program);

	static LRESULT CALLBACK WindowProc(
		HWND hWnd, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam);

protected:
    /**
     * @brief Do anything that needs to be done immediately before rendering
     * @param context Graphical context
     */
    void preFrame(Context* context);

    /**
     * @brief Do anything that needs to be done immediately after rendering
     * A prime example of this is the swapping of back-buffer to the display
     * @param context Graphical context
     */
    void postFrame(Context* context);


private:
	HWND m_HWnd;
	HDC m_HDC;
	HGLRC m_HRC; 
	WORD m_P1Ctrl, m_P2Ctrl;

	struct WIN32_EVENT
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
	};
	std::queue<WIN32_EVENT> m_EventQueue;
    std::mutex m_EventMtx;              //!< Mutex for controlling queue access

    Context m_Context;              //!< Custom context
    bool m_Loaded;                  //!< If false, constructor failed
    std::atomic<bool> m_Running;    //!< Whilst true, keep threads running

	void postWin32EventToQueue(WIN32_EVENT& wievent);

	void handleWin32EventQueue(Program* program);

	void handleXInput();

	void tempWindowStuff(const std::string& title, HINSTANCE hInstance);

	void initGL(HWND hWnd, bool vSync = false);

	HWND createWindow(
		const std::string& title,
		unsigned int width, unsigned int height,
		bool show,
		HINSTANCE hInstance);

	HWND createWindow(const std::string& title, HINSTANCE hInstance);

	void destroyWindow(HWND hWnd);
};
