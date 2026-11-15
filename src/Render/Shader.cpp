#include "Shader.h"


#include <SDL3/SDL.h>
#include <iostream>


Shader::Shader(const char* path, SDL_GPUDevice* device, SDL_GPUShaderCreateInfo* create_info)
{
	CreateShader(path, device, create_info);
}

Shader::~Shader()
{
}

SDL_GPUShader* Shader::GetShader()
{
    return mp_shader;
}

bool Shader::CreateShader(const char* path, SDL_GPUDevice* device, SDL_GPUShaderCreateInfo* create_info)
{
	size_t datasize;

	void* buffer = SDL_LoadFile(path, &datasize);
	if (buffer == NULL)
	{
		std::cout << "Can't read file : " << path << std::endl;
		return false;
	}

	create_info->code = static_cast<Uint8*>(buffer);
	create_info->code_size = datasize;

	mp_shader = SDL_CreateGPUShader(device, create_info);
	if (!mp_shader)
	{
		std::cout << "Can't create shader : " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_free(buffer);
	return true;
}

void Shader::Destroy(SDL_GPUDevice* device)
{
	SDL_ReleaseGPUShader(device, mp_shader);
}