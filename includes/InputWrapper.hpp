#ifndef __INPUT_WRAPPER_HPP__
#define __INPUT_WRAPPER_HPP__

class InputWrapper
{
public:
	InputWrapper() = default;
	InputWrapper(InputWrapper const&) = delete;
	InputWrapper(InputWrapper&&) = delete;
	~InputWrapper() = default;
	InputWrapper&	operator=(InputWrapper const&) = delete;
	InputWrapper&	operator=(InputWrapper&&) = delete;
};

#endif