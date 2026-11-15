#pragma once

struct SDL_Window;
struct SDL_GPUDevice;
typedef unsigned int SDL_GPUShaderFormat;

class GPU
{
private:

	SDL_GPUDevice* mp_device = nullptr;

public:

	static GPU& GetInstance()
	{
		static GPU instance;
		return instance;
	}

	GPU(SDL_Window* window, SDL_GPUShaderFormat format_flags, bool debug_mode, const char* name = 0);
	GPU() = default;
	
	~GPU();


	SDL_GPUDevice* GetDevice();

	bool CreateDevice(SDL_Window* window, SDL_GPUShaderFormat format_flags, bool debug_mode, const char* name = 0);

	void Destroy(SDL_Window* window);
};