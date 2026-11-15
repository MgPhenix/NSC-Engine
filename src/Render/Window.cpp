#include "Window.h"

#include <SDL3/SDL.h>
#include <iostream>


Window::Window(const char* windowName, int windW, int windH, SDL_WindowFlags flags)
{
	CreateWindow(windowName, windW, windH, flags);
}

Window::~Window()
{
	Destroy();
}

SDL_Window* Window::GetWindow()
{
    return mp_window;
}

bool Window::CreateWindow(const char* windowName, int windW, int windH, SDL_WindowFlags flags)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return false;
	}

	mp_window = SDL_CreateWindow(windowName, windW, windH, flags);

	if (!mp_window)
	{
		std::cout << "SDL_CreateWindow_Error : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Window::Destroy()
{
	SDL_DestroyWindow(mp_window);
	width = 0;
	height = 0;
}