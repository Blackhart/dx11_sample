#include "../includes/InputWrapper.hpp"

uint8_t const __MAX_INPUT__ = 255;

void	InputWrapper::Initialize()
{
	for (uint8_t i = 0; i < __MAX_INPUT__; ++i)
		__keys[i] = false;
}

void	InputWrapper::KeyDown(uint8_t const input)
{
	if (input < __MAX_INPUT__)
		__keys[input] = true;
}

void	InputWrapper::KeyUp(uint8_t const input)
{
	if (input < __MAX_INPUT__)
		__keys[input] = false;
}

bool	InputWrapper::IsKeyDown(uint8_t const key) const
{
	return ((key < __MAX_INPUT__) ? __keys[key] : false);
}