#include "GPU.h"


#include <SDL3/SDL.h>
#include <iostream>


GPU::GPU(SDL_Window* window, SDL_GPUShaderFormat format_flags, bool debug_mode, const char* name)
{
	CreateDevice(window, format_flags, debug_mode, name);
}

GPU::~GPU()
{
	SDL_DestroyGPUDevice(mp_device);
}

SDL_GPUDevice* GPU::GetDevice()
{
	return mp_device;
}

bool GPU::CreateDevice(SDL_Window* window, SDL_GPUShaderFormat format_flags, bool debug_mode, const char* name)
{
	mp_device = SDL_CreateGPUDevice(format_flags, debug_mode, name);
	if (mp_device == NULL)
	{
		std::cout << "Error creating GPU Device : " << SDL_GetError() << std::endl;
		return false;
	}

	if (!SDL_ClaimWindowForGPUDevice(mp_device, window))
	{
		std::cout << "Error when claiming window for GPU Device : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void GPU::Destroy(SDL_Window* window)
{
	SDL_ReleaseWindowFromGPUDevice(mp_device, window);
	SDL_DestroyGPUDevice(mp_device);
}