#ifndef __INPUT_WRAPPER_HPP__
#define __INPUT_WRAPPER_HPP__

#include <cstdint>

extern uint8_t const __MAX_INPUT__;

class InputWrapper
{
public:
	InputWrapper() = default;
	InputWrapper(InputWrapper const&) = delete;
	InputWrapper(InputWrapper&&) = delete;
	~InputWrapper() = default;
	InputWrapper&	operator=(InputWrapper const&) = delete;
	InputWrapper&	operator=(InputWrapper&&) = delete;
	void	Initialize();
	void	KeyDown(uint8_t const);
	void	KeyUp(uint8_t const);
	bool	IsKeyDown(uint8_t const) const;
private:
	bool	__keys[256];
};

#endif