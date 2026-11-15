#pragma once

struct SDL_GPUGraphicsPipeline;
struct SDL_GPUShader;
struct SDL_GPUDevice;
struct SDL_Window;

class Pipeline
{
private:
	
	SDL_GPUGraphicsPipeline* mp_pipeline = nullptr;

public:

	Pipeline(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader);
	Pipeline() = default;

	~Pipeline();

	SDL_GPUGraphicsPipeline* GetPipeline();

	bool CreatePipeline(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader);

	void Destroy();
};