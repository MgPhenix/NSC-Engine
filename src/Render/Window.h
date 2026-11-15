#pragma once

struct SDL_Window;
typedef unsigned long long SDL_WindowFlags;

class Window
{
private:

	SDL_Window* mp_window = nullptr;

	int width  = 0;
	int height = 0;

public:

	Window(const char* windowName, int windW, int windH, SDL_WindowFlags flags);
	Window() = default;

	~Window();


	SDL_Window* GetWindow();

	bool CreateWindow(const char* windowName, int windW, int windH, SDL_WindowFlags flags);

	void Destroy();
};