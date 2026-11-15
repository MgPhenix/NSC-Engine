#pragma once

struct SDL_GPUShader;
struct SDL_GPUDevice;
struct SDL_GPUShaderCreateInfo;

class Shader
{
private:

	SDL_GPUShader* mp_shader = nullptr;

public:
	
	Shader(const char* path, SDL_GPUDevice* device, SDL_GPUShaderCreateInfo* create_info);
	Shader() = default;

	~Shader();

	SDL_GPUShader* GetShader();

	bool CreateShader(const char* path, SDL_GPUDevice* device, SDL_GPUShaderCreateInfo* create_info);

	void Destroy(SDL_GPUDevice* device);
};