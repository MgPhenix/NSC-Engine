#include "NSCEngine.h"
#include <SDL3/SDL.h>

void Engine::Init(const char* title, int width, int height)
{
	m_window.CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
	m_render.InitRenderSystem(&m_window);
}

void Engine::Update()
{

	while (m_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{

		}
	}
}