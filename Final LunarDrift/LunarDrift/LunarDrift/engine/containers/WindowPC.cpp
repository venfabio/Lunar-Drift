#include "WindowPC.h"
#include "../input/InputMap.h"
#include "../comms/MessageSystem.h"
#include "../comms/predefined/ExitMessage.h"
#include "../../engine/exceptions/Exceptions.h"
#include <windowsx.h>
#include <Xinput.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#pragma comment(lib, "Xinput.lib")

WindowPC::WindowPC(
	const std::string& title,
	HINSTANCE hInstance,
	LPSTR lpCmdLine,
	int nCmdShow,
	unsigned int width,
	unsigned int height)
    : m_Loaded(false),
      m_Running(true)
{
	// Setup the Win32 metaclass
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowPC::WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(2));
	//wc.hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(3));
	wc.lpszClassName = title.c_str();

	if (RegisterClassEx(&wc) == 0)
		throw std::exception("Could not register window class");


	// Uncomment to request higher priority;
	// this can help reduce the impact of OS scheduling-related bottlenecks
	//SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// Gotta do this first
	tempWindowStuff(title, hInstance);

	// Create the final window
	HWND hWnd = createWindow(title, width, height, true, hInstance);

	// Permits the use of local instances in WinProc
	SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// Store final window handle
    m_HWnd = hWnd;
	// And initialize OpenGL on it
	initGL(hWnd);

	OutputDebugString((getGPUName() + "\r\n").c_str());	// Debug GPU device selected

    glGetError();   // Clear error flags
    m_Context.setDimensions(width, height);
}

WindowPC::~WindowPC()
{
	if (m_HRC != nullptr)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_HRC);
	}

	if (m_HDC != nullptr)
		ReleaseDC(m_HWnd, m_HDC);

	if (m_HWnd != nullptr)
		destroyWindow(m_HWnd);
}


HWND WindowPC::createWindow(
	const std::string& title,
	unsigned int width, unsigned int height,
	bool show,
	HINSTANCE hInstance)
{
	// Determine the resolution of the clients desktop screen.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// We want an overlapped window
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	DWORD dwExStyle = 0;

	// We use this function to clamp the OpenGL context to the window edges
	RECT dimensions;
	dimensions.left = 0;     dimensions.right = width;
	dimensions.top = 0;      dimensions.bottom = height;
	AdjustWindowRectEx(&dimensions, dwStyle, false, dwExStyle);

	// Create main window
	HWND hWnd = CreateWindowEx(
		dwExStyle,
		title.c_str(),
		title.c_str(),
		dwStyle,
		(screenWidth / 2) - (dimensions.right / 2),
		(screenHeight / 2) - (dimensions.bottom / 2),
		dimensions.right - dimensions.left,
		dimensions.bottom - dimensions.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (show)
	{
		ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
	}
	else
	{
		ShowWindow(hWnd, SW_HIDE);
	}

	return hWnd;
}

HWND WindowPC::createWindow(const std::string& title, HINSTANCE hInstance)
{
	// Determine the resolution of the clients desktop screen.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// If full screen set the screen to maximum size of the users desktop
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = static_cast<DWORD>(screenWidth);
	dmScreenSettings.dmPelsHeight = static_cast<DWORD>(screenHeight);
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// Change the display settings to full screen.
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	// We use this function to clamp the OpenGL context to the window edges
	RECT dimensions;
	dimensions.left = 0;     dimensions.right = (long)screenWidth;
	dimensions.top = 0;      dimensions.bottom = (long)screenHeight;
	AdjustWindowRectEx(&dimensions, dwStyle, false, dwExStyle);

	// Create main window
	HWND hWnd = CreateWindowEx(
		dwExStyle,
		title.c_str(),
		title.c_str(),
		dwStyle,
		dimensions.left,
		dimensions.top,
		dimensions.right - dimensions.left,
		dimensions.bottom - dimensions.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	ShowWindow(hWnd, SW_SHOW);

	m_Context.setDimensions(screenWidth, screenHeight);

	return hWnd;
}

void WindowPC::destroyWindow(HWND hWnd)
{
    if (hWnd == nullptr)
        return;
	DestroyWindow(hWnd);
}

void WindowPC::tempWindowStuff(const std::string& title, HINSTANCE hInstance)
{
	HWND tempHWnd = createWindow(title, 2, 2, false, hInstance);

	HDC hDC = GetDC(tempHWnd);
	if (hDC == nullptr)
		throw std::exception("Could not acquire temporary device context");

	// Make a default pixel format
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flag
		PFD_TYPE_RGBA,             // The kind of framebuffer
		32,                        // Colordepth of the framebuffer
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        // Number of bits for the depthbuffer
		8,                         // Number of bits for the stencilbuffer
		0,                         // Number of Aux buffers in the framebuffer
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int letWindowsChoose = ChoosePixelFormat(hDC, &pfd);
	if (SetPixelFormat(hDC, letWindowsChoose, &pfd) != TRUE)
	{
		DWORD err = GetLastError();
		throw std::exception("Could not acquire temporary pixel format");
	}

	HGLRC hRC = wglCreateContext(hDC);
	if (hRC == nullptr)
		throw std::exception("Could not create temporary rendering context");

	if (wglMakeCurrent(hDC, hRC) != TRUE)
		throw std::exception("Could not set temporary rendering context");

	// The piece de la resistance: setting up GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::exception("Could not initialize GLEW");

	// Kill temporary window
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);
	ReleaseDC(tempHWnd, hDC);

	destroyWindow(tempHWnd);
}

void addAttribute(std::vector<int>* attribList, int attrib, int value)
{
	attribList->push_back(attrib);
	attribList->push_back(value);
}

void WindowPC::initGL(HWND hWnd, bool vSync)
{
	PIXELFORMATDESCRIPTOR pfd;

	HDC hDC = GetDC(hWnd);
	if (hDC == nullptr)
		throw std::exception("Could not acquire device context");

	// Fill out the context attributes
	std::vector<int> attribList;
	addAttribute(&attribList, WGL_DRAW_TO_WINDOW_ARB, true);
	addAttribute(&attribList, WGL_SUPPORT_OPENGL_ARB, true);
	addAttribute(&attribList, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB);
	addAttribute(&attribList, WGL_DOUBLE_BUFFER_ARB, true);
	addAttribute(&attribList, WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB);
	addAttribute(&attribList, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB);
	addAttribute(&attribList, WGL_COLOR_BITS_ARB, GLCONFIG_COLOR_BITS);
	addAttribute(&attribList, WGL_ALPHA_BITS_ARB, GLCONFIG_ALPHA_BITS);
	addAttribute(&attribList, WGL_DEPTH_BITS_ARB, GLCONFIG_DEPTH_BITS);
	addAttribute(&attribList, WGL_STENCIL_BITS_ARB, GLCONFIG_STENCIL_BITS);
	attribList.push_back(0);    // Null-terminate

	// Try and acquire a GL-valid pixel format for the window
	int formats[1];
	unsigned int numFormats;
	if (wglChoosePixelFormatARB(
		hDC, &attribList[0], nullptr, 1, formats, &numFormats) != TRUE)
	{
		throw std::exception("Could not obtain a valid pixel format");
	}

	// Now try and *set* said format
	if (SetPixelFormat(hDC, formats[0], &pfd) != TRUE)
		throw std::exception("Could not set the GL-compatible pixel format");

	// Now we want our OpenGL version set
	attribList.clear();
	addAttribute(&attribList, WGL_CONTEXT_MAJOR_VERSION_ARB, GLCONFIG_MAJOR_VERSION);
	addAttribute(&attribList, WGL_CONTEXT_MINOR_VERSION_ARB, GLCONFIG_MINOR_VERSION);
	attribList.push_back(0);    // Null-terminate

	HGLRC hRC = wglCreateContextAttribsARB(hDC, nullptr, &attribList[0]);
	if (hRC == nullptr)
		throw std::exception("Could not create OpenGL rendering context");

	/// TODO: Multisampling!

	// Set the contexts as currently active
	m_HDC = hDC;
	m_HRC = hRC;

	// Activate together
	if (wglMakeCurrent(m_HDC, m_HRC) != TRUE)
		throw std::exception("Could not activate final OpenGL context");

	// Set VSync
	BOOL result;
	if (vSync)
		result = wglSwapIntervalEXT(1);
	else
		result = wglSwapIntervalEXT(0);

	if (result != TRUE)
		throw std::exception("Could not set VSync status");
}

ExitStatus WindowPC::run(std::unique_ptr<Program> program)
{
    assert(program);

    //Window::run(program);

	ZeroMemory(&m_P1Ctrl, sizeof(m_P1Ctrl));
	ZeroMemory(&m_P2Ctrl, sizeof(m_P2Ctrl));

	MessageSystem::getInstance().subscribe("Exit", this);

	std::thread simThread;
	if (USE_MULTITHREADING)
		simThread = std::thread(&WindowPC::simulationThread, this, program.get());

	renderingThread(&m_Context, program.get());

	MessageSystem::getInstance().unsubscribe("Exit", this);

	if (USE_MULTITHREADING)
		simThread.join();
	program.reset();
	destroyWindow(m_HWnd);

    return ExitStatus::Success;
}

void WindowPC::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType() == "Exit")
	{
		m_Running = false;
	}
}

void WindowPC::simulationThread(Program* program)
{
    double lastTime = clock();
    while(m_Running)
    {
        double now = clock();
        double dt = static_cast<double>(now - lastTime) / CLOCKS_PER_SEC;
        lastTime = now;
        program->step(dt);
    }
}

void WindowPC::renderingThread(Context* context, Program* program)
{
    try
	{
		double lastTime = clock();
        while(m_Running)
        {
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (!USE_MULTITHREADING)
			{
				double now = clock();
				double dt = static_cast<double>(now - lastTime) / CLOCKS_PER_SEC;
				lastTime = now;
				program->step(dt);
			}

            preFrame(context);
            program->frame(context);
			postFrame(context);

			handleWin32EventQueue(program);
			handleXInput();
        }
    }
    catch(ShaderException& ex)
    {
        std::string msg(ex.getFile() + " (" + ex.getComponent() + " " +
                        ex.getStage() + " failed)\r\n" + ex.getErrorLog());

        MessageBox(nullptr, msg.c_str(), "Shader Loading Error", MB_ICONERROR);
        
        destroyWindow(m_HWnd);
    } catch(OpenGLException& ex)
    {
        std::string msg(ex.getReport() + " (" +
                        ex.glErrorCodeToString(ex.getGLErrorCode()) + ")");

        MessageBox(nullptr, msg.c_str(), "OpenGL Error", MB_ICONERROR);

        destroyWindow(m_HWnd);
    } catch(ConfigurationException& ex)
    {
        std::string msg("A configuration setting is invalid!\r\n" +
                        ex.getSetting() + " " + ex.getError());

        MessageBox(nullptr, msg.c_str(), "Configuration Error", MB_ICONERROR);

        destroyWindow(m_HWnd);
    } catch(FileIOException& ex)
    {
        std::string msg(ex.getFile() + ": " + ex.getError());

		MessageBox(nullptr, msg.c_str(), "File I/O Error", MB_ICONERROR);

        destroyWindow(m_HWnd);
    } catch(UnsupportedFeatureException& ex)
    {
        std::string msg("A feature is unsupported!\r\n" +
                        ex.getFeature() + ": " + ex.getReason());

		MessageBox(nullptr, msg.c_str(), "Unsupported Feature", MB_ICONERROR);

        destroyWindow(m_HWnd);
    } catch(std::exception& ex)
    {
		MessageBox(nullptr, ex.what(), "Unhandled Exception", MB_ICONERROR);

        destroyWindow(m_HWnd);
    } catch(...)
    {
        std::cerr << "Unknown exception" << std::endl;
        destroyWindow(m_HWnd);
    }
}

void WindowPC::preFrame(Context* context)
{

}

void WindowPC::postFrame(Context* context)
{
	SwapBuffers(m_HDC);
}


void WindowPC::postWin32EventToQueue(WIN32_EVENT& wievent)
{
	m_EventQueue.push(wievent);
}

void WindowPC::handleWin32EventQueue(Program* program)
{
	while (m_EventQueue.size() > 0)
	{
		WIN32_EVENT wievent = m_EventQueue.front();
		m_EventQueue.pop();

		switch (wievent.message)
		{
			case WM_KEYDOWN:
				InputMap::getInstance().getKeyboard()->setKeyDown(wievent.wParam);
				//m_Program->onKeyDown(wievent.wParam);
				break;

			case WM_KEYUP:
				InputMap::getInstance().getKeyboard()->setKeyUp(wievent.wParam);
				//m_Program->onKeyUp(wievent.wParam);
				break;

			case WM_LBUTTONDOWN:
			{
				/*m_Program->onTouchClick(
					GET_X_LPARAM(wievent.lParam),
					GET_Y_LPARAM(wievent.lParam)
					);*/
			}
			break;

			case WM_SIZE:
			{
				program->setSize(
					&m_Context,
					LOWORD(wievent.lParam), 
					HIWORD(wievent.lParam));
			}
			break;

			case WM_CLOSE:
			{
				m_Running = false;
			}
			break;
		}
	}
}

LRESULT CALLBACK WindowPC::WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam) 
{
	WindowPC* pThis = reinterpret_cast<WindowPC*>(
		GetWindowLongPtrW(hWnd, GWLP_USERDATA));

	switch (message)
	{
		case WM_KEYDOWN:
		{
			WIN32_EVENT wievent{ message, wParam, lParam };
			pThis->postWin32EventToQueue(wievent);
		}
		break;

		case WM_KEYUP:
		{
			WIN32_EVENT wievent{ message, wParam, lParam };
			pThis->postWin32EventToQueue(wievent);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			WIN32_EVENT wievent{ message, wParam, lParam };
			pThis->postWin32EventToQueue(wievent);
		}
		break;

		case WM_SIZE:
		{
			if (pThis == nullptr)
				break;

			WIN32_EVENT wievent{ message, wParam, lParam };
			pThis->postWin32EventToQueue(wievent);
		}
		break;

		case WM_CLOSE:
		{
			WIN32_EVENT wievent{ message, wParam, lParam };
			pThis->postWin32EventToQueue(wievent);
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowPC::handleXInput()
{
	Keyboard* kbd = InputMap::getInstance().getKeyboard();

	XINPUT_STATE state;
	DWORD result = XInputGetState(0, &state);
	if (result == ERROR_SUCCESS)
	{
		WORD newState = state.Gamepad.wButtons;
		if (((newState & XINPUT_GAMEPAD_DPAD_UP) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_UP) > 0))
		{
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PitchDown"));
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Up"));
		}
		if (((newState & XINPUT_GAMEPAD_DPAD_DOWN) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_DOWN) > 0))
		{
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PitchUp"));
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Down"));
		}
		if (((newState & XINPUT_GAMEPAD_DPAD_LEFT) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_LEFT) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("YawLeft"));
		if (((newState & XINPUT_GAMEPAD_DPAD_RIGHT) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_RIGHT) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("YawRight"));
		if (((newState & XINPUT_GAMEPAD_A) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_A) > 0))
		{
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Brake"));
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Enter"));
		}
		if (((newState & XINPUT_GAMEPAD_B) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_B) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanLeft"));
		if (((newState & XINPUT_GAMEPAD_X) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_X) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanRight"));
		if (((newState & XINPUT_GAMEPAD_Y) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_Y) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Accelerate"));
		if (((newState & XINPUT_GAMEPAD_START) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_START) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Pause"));
		if (((newState & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanUp"));
		if (((newState & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanDown"));
		if (((newState & XINPUT_GAMEPAD_BACK) > 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_BACK) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Back"));

		if (((newState & XINPUT_GAMEPAD_DPAD_UP) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_UP) == 0))
		{
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PitchDown"));
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Up"));
		}
		if (((newState & XINPUT_GAMEPAD_DPAD_DOWN) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_DOWN) == 0))
		{
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PitchUp"));
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Down"));
		}
		if (((newState & XINPUT_GAMEPAD_DPAD_LEFT) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_LEFT) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("YawLeft"));
		if (((newState & XINPUT_GAMEPAD_DPAD_RIGHT) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_DPAD_RIGHT) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("YawRight"));
		if (((newState & XINPUT_GAMEPAD_A) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_A) == 0))
		{
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Brake"));
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Enter"));
		}
		if (((newState & XINPUT_GAMEPAD_B) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_B) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanLeft"));
		if (((newState & XINPUT_GAMEPAD_X) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_X) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanRight"));
		if (((newState & XINPUT_GAMEPAD_Y) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_Y) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Accelerate"));
		if (((newState & XINPUT_GAMEPAD_START) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_START) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Pause"));
		if (((newState & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanUp"));
		if (((newState & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanDown"));
		if (((newState & XINPUT_GAMEPAD_BACK) == 0) && ((m_P1Ctrl & XINPUT_GAMEPAD_BACK) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Back"));

		m_P1Ctrl = newState;
	}
	result = XInputGetState(1, &state);
	if (result == ERROR_SUCCESS)
	{
		WORD newState = state.Gamepad.wButtons;
		if (((newState & XINPUT_GAMEPAD_DPAD_UP) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_UP) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PitchDown2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_DOWN) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_DOWN) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PitchUp2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_LEFT) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_LEFT) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("YawLeft2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_RIGHT) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_RIGHT) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("YawRight2"));
		if (((newState & XINPUT_GAMEPAD_A) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_A) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Brake2"));
		if (((newState & XINPUT_GAMEPAD_B) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_B) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanLeft2"));
		if (((newState & XINPUT_GAMEPAD_X) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_X) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanRight2"));
		if (((newState & XINPUT_GAMEPAD_Y) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_Y) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Accelerate2"));
		if (((newState & XINPUT_GAMEPAD_START) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_START) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("Pause"));
		if (((newState & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanUp2"));
		if (((newState & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0))
			kbd->setKeyDown(kbd->getKeyAssociatedWithMap("PanDown2"));

		if (((newState & XINPUT_GAMEPAD_DPAD_UP) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_UP) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PitchDown2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_DOWN) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_DOWN) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PitchUp2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_LEFT) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_LEFT) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("YawLeft2"));
		if (((newState & XINPUT_GAMEPAD_DPAD_RIGHT) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_DPAD_RIGHT) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("YawRight2"));
		if (((newState & XINPUT_GAMEPAD_A) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_A) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Brake2"));
		if (((newState & XINPUT_GAMEPAD_B) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_B) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanLeft2"));
		if (((newState & XINPUT_GAMEPAD_X) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_X) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanRight2"));
		if (((newState & XINPUT_GAMEPAD_Y) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_Y) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Accelerate2"));
		if (((newState & XINPUT_GAMEPAD_START) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_START) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("Pause"));
		if (((newState & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanUp2"));
		if (((newState & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0) && ((m_P2Ctrl & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0))
			kbd->setKeyUp(kbd->getKeyAssociatedWithMap("PanDown2"));

		m_P2Ctrl = newState;
	}
}
