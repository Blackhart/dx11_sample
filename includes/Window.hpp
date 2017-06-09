#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdint>
#include <memory>
#include <new>
#include "InputWrapper.hpp"

enum eWindowMode
{
	WINDOWED,
	FULL_SCREEN
};
typedef eWindowMode	WindowMode;

class Window
{
public:
	Window();
	Window(Window const&) = delete;
	Window(Window&&) = delete;
	~Window() = default;
	Window&		operator=(Window const&) = delete;
	Window&		operator=(Window&&) = delete;
	bool				Initialize();
	void				Uninitialize();
	bool				Run();
	uint16_t			GetWindowWidth() const;
	uint16_t			GetWindowHeight() const;
	HWND				GetWindow() const;
	WindowMode			GetWindowMode() const;
	LRESULT CALLBACK	MessageHandler(HWND const hwnd, UINT const umsg, WPARAM const wparam, LPARAM const lparam);
private:
	void	InitializeWindows();
	void	UninitializeWindows();
private:
	LPCWSTR								__appName;
	HINSTANCE							__hinstance;
	HWND								__hwnd;
	std::unique_ptr<InputWrapper>		__input;
	WindowMode							__windowMode;
	uint16_t							__windowWidth;
	uint16_t							__windowHeight;
};

extern Window*	WindowInst;

extern LRESULT	CALLBACK	WndProc(HWND const hwnd, UINT const umessage, WPARAM const wparam, LPARAM const lparam);

#endif