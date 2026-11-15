#pragma once
#include "Pipeline.h"
#include "Shader.h"
#include "Sampler.h"
#include <vector>


class Window;
class Shape;
struct SDL_GPUCommandBuffer;
struct SDL_GPURenderPass;


struct RenderFrame
{
	SDL_GPUCommandBuffer* cmd = nullptr;
	SDL_GPURenderPass* render_pass = nullptr;

	bool can_be_use = false;

	RenderFrame() = default;
	~RenderFrame() = default;

	void InitFrame(Window* window);
	void EndRenderPassAndSubmitCommand();
};


class RenderSystem
{
private:

	Pipeline m_texture_pipeline;
	Pipeline m_color_pipeline;

	Shader m_vertex_shader;
	Shader m_pixel_shader;
	Shader m_pixel_color_shader;

	Sampler m_sampler;
	Sampler m_pixel_art_sampler;

	bool CreateShader();

	void RenderShape(SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass* render_pass, Shape* shape, float x, float y);
	void BuilSharedBuffer(const std::vector<Shape*>& shapes);

public:

	RenderSystem() = default;
	RenderSystem(Window* window);

	~RenderSystem();
	void Destroy();

	void InitRenderSystem(Window* window);

	void Draw(Window* window, const std::vector<Shape*>& shapes);
	void Draw(Window* window, RenderFrame* renderer, const std::vector<Shape*>& shapes);
	void Draw(Window* window, RenderFrame* renderer, Shape* shape);
};


RenderSystem* CreateRenderSystem(Window* window);

RenderFrame* NewFrame(Window* window);

void EndAndSubmit(RenderFrame* renderer);