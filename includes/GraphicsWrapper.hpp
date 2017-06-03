#ifndef __GRAPHICS_WRAPPER_HPP__
#define __GRAPHICS_WRAPPER_HPP__

class GraphicsWrapper
{
public:
	GraphicsWrapper() = default;
	GraphicsWrapper(GraphicsWrapper const&) = delete;
	GraphicsWrapper(GraphicsWrapper&&) = delete;
	~GraphicsWrapper() = default;
	GraphicsWrapper&	operator=(GraphicsWrapper const&) = delete;
	GraphicsWrapper&	operator=(GraphicsWrapper&&) = delete;
};

#endif