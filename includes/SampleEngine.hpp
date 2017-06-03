#ifndef __SAMPLE_ENGINE_HPP__
#define __SAMPLE_ENGINE_HPP__

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdint>
#include <memory>
#include "GraphicsWrapper.hpp"
#include "InputWrapper.hpp"

enum eWindowMode
{
	WINDOWED,
	FULL_SCREEN
};
typedef eWindowMode	WindowMode;

class SampleEngine
{
public:
	SampleEngine() = default;
	SampleEngine(SampleEngine const&) = delete;
	SampleEngine(SampleEngine&&) = delete;
	~SampleEngine() = default;
	SampleEngine&	operator=(SampleEngine const&) = delete;
	SampleEngine&	operator=(SampleEngine&&) = delete;
	bool	Initialize();
	void	Uninitialize();
	void	Run();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool	Render();
	void	InitializeWindows(uint16_t&, uint16_t&);
	void	UninitializeWindows();
private:
	LPCWSTR				__appName;
	HINSTANCE			__hinstance;
	HWND				__hwnd;
	std::unique_ptr<InputWrapper>		__input;
	std::unique_ptr<GraphicsWrapper>	__graphics;
	WindowMode	__windowMode;
};

static LRESULT	CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

static SampleEngine*	SampleEngineInst = nullptr;

#endif