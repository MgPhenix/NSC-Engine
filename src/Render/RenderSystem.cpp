#include "RenderSystem.h"

#include <ToolsBox/Matrice.h>

#include "GPU.h"
#include "Window.h"
#include "Shape.h"
#include <SDL3/SDL.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace
{
	Mat4x4f projection = Ortho(0.0f, 800.f, 800.f, 0.0f, -1.0f, 1.f);

	bool AcquireCmdAndRenderPass(Window* window, SDL_GPUCommandBuffer** cmd, SDL_GPURenderPass** render_pass, FColor color = FColor{ 0.1f, 0.1f, 0.15f, 1.0f })
	{
		SDL_GPUDevice* device = GPU::GetInstance().GetDevice();

		*cmd = SDL_AcquireGPUCommandBuffer(device);
		SDL_GPUTexture* swapchain_texture;
		if (!SDL_WaitAndAcquireGPUSwapchainTexture(*cmd, window->GetWindow(), &swapchain_texture, NULL, NULL))
		{
			std::cout << "Error waiting for swapchain texture : " << SDL_GetError() << std::endl;
			return false;
		}

		SDL_GPUColorTargetInfo color_target = {};
		color_target.texture = swapchain_texture;
		color_target.clear_color = SDL_FColor(color.r, color.g, color.b, color.a);
		color_target.load_op = SDL_GPU_LOADOP_CLEAR;
		color_target.store_op = SDL_GPU_STOREOP_STORE;

		*render_pass = SDL_BeginGPURenderPass(*cmd, &color_target, 1, NULL);

		return true;
	}
}


#pragma region RenderSystem

bool RenderSystem::CreateShader()
{
	// VERTEX // 

	SDL_GPUDevice* device = GPU::GetInstance().GetDevice();


	SDL_GPUShaderCreateInfo vertex_info = {};
	vertex_info.entrypoint = "VSMain";
	vertex_info.format = SDL_GPU_SHADERFORMAT_DXIL;
	vertex_info.stage = SDL_GPU_SHADERSTAGE_VERTEX;

	vertex_info.num_samplers = 0;
	vertex_info.num_storage_textures = 0;
	vertex_info.num_storage_buffers = 0;
	vertex_info.num_uniform_buffers = 1;

	if (!m_vertex_shader.CreateShader("../../shader/vertex.dxil", device, &vertex_info))
	{
		std::cout << "Can't create vertex shader" << std::endl;
		return false;
	}

	// PIXEL //

	SDL_GPUShaderCreateInfo pixel_info = {};
	pixel_info.entrypoint = "PSMain";
	pixel_info.format = SDL_GPU_SHADERFORMAT_DXIL;
	pixel_info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

	pixel_info.num_samplers = 1;
	pixel_info.num_storage_textures = 0;
	pixel_info.num_storage_buffers = 0;
	pixel_info.num_uniform_buffers = 0;


	if (!m_pixel_shader.CreateShader("../../shader/pixel.dxil", device, &pixel_info))
	{
		std::cout << "Can't create pixel shader" << std::endl;
		return false;
	}

	//Pixel Color

	SDL_GPUShaderCreateInfo pixel_color_info = {};
	pixel_color_info.entrypoint = "PSMainColor";
	pixel_color_info.format = SDL_GPU_SHADERFORMAT_DXIL;
	pixel_color_info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

	pixel_color_info.num_samplers = 0;
	pixel_color_info.num_storage_textures = 0;
	pixel_color_info.num_storage_buffers = 0;
	pixel_color_info.num_uniform_buffers = 0;


	if (!m_pixel_color_shader.CreateShader("../../shader/pixel_color.dxil", device, &pixel_color_info))
	{
		std::cout << "Can't create color pixel shader" << std::endl;
		return false;
	}

	return true;
}

void RenderSystem::RenderShape(SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass* render_pass, Shape* shape, float x, float y)
{
	Mat4x4 mat_translate = Translate(x, y, 0.f);
	Mat4x4 model = projection * mat_translate;

	SDL_PushGPUVertexUniformData(cmd, 0, &model, sizeof(model));


	if (shape->HasTexture())
		SDL_BindGPUGraphicsPipeline(render_pass, m_texture_pipeline.GetPipeline());
	else
		SDL_BindGPUGraphicsPipeline(render_pass, m_color_pipeline.GetPipeline());


	DrawData data = shape->GetDrawData();

	SDL_GPUBufferBinding vertex_binding = {};
	vertex_binding.buffer = data.vertex_buffer;
	vertex_binding.offset = 0;

	SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_binding, 1);

	SDL_GPUBufferBinding index_binding = {};
	index_binding.buffer = data.index_buffer;
	index_binding.offset = 0;

	SDL_BindGPUIndexBuffer(render_pass, &index_binding, SDL_GPUIndexElementSize::SDL_GPU_INDEXELEMENTSIZE_16BIT);

	if (shape->HasTexture())
	{
		SDL_GPUTextureSamplerBinding sampler_binding = {};

		if(shape->IsPixelArt())
			sampler_binding.sampler = m_pixel_art_sampler.GetSampler();
		else
			sampler_binding.sampler = m_sampler.GetSampler();
		
		sampler_binding.texture = data.texture;

		SDL_BindGPUFragmentSamplers(render_pass, 0, &sampler_binding, 1);
	}

	SDL_DrawGPUIndexedPrimitives(render_pass, data.num_indices, 1, 0, 0, 0);
}

void RenderSystem::BuilSharedBuffer(const std::vector<Shape*>& shapes)
{
	// TODO One day...
}

RenderSystem::RenderSystem(Window* window)
{
	InitRenderSystem(window);
}

RenderSystem::~RenderSystem()
{
	Destroy();
}

void RenderSystem::Destroy()
{
	m_texture_pipeline.Destroy();
	m_color_pipeline.Destroy();
}

void RenderSystem::InitRenderSystem(Window* window)
{
	if (!GPU::GetInstance().CreateDevice(window->GetWindow(), SDL_GPU_SHADERFORMAT_DXIL, true, NULL))
	{
		std::cout << "Can't create GPU Device" << std::endl;
		return;
	}

	if (!CreateShader())
	{
		std::cout << "Can't create shader" << std::endl;
		return;
	}

	SDL_GPUDevice* device = GPU::GetInstance().GetDevice();

	if (!m_texture_pipeline.CreatePipeline(
		window->GetWindow(),
		device,
		m_vertex_shader.GetShader(),
		m_pixel_shader.GetShader()))
	{
		std::cout << "Can't create texture pipeline" << std::endl;
		return;
	}

	if (!m_color_pipeline.CreatePipeline(
		window->GetWindow(),
		device,
		m_vertex_shader.GetShader(),
		m_pixel_color_shader.GetShader()))
	{
		std::cout << "Can't create color pipeline" << std::endl;
		return;
	}

	m_sampler.CreateSampler();
	m_pixel_art_sampler.CreateSampler(true);

	m_vertex_shader.Destroy(device);
	m_pixel_shader.Destroy(device);
	m_pixel_color_shader.Destroy(device);
}

void RenderSystem::Draw(Window* window, const std::vector<Shape*>& shapes)
{
	SDL_GPUCommandBuffer* cmd = nullptr;
	SDL_GPURenderPass* render_pass = nullptr;

	AcquireCmdAndRenderPass(window, &cmd, &render_pass);

	for (Shape* shape : shapes)
		RenderShape(cmd, render_pass, shape, 400.f, 400.f);
	

	SDL_EndGPURenderPass(render_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
}

void RenderSystem::Draw(Window* window, RenderFrame* renderer, const std::vector<Shape*>& shapes)
{
	for (Shape* shape : shapes)
		RenderShape(renderer->cmd, renderer->render_pass, shape, 400.f, 400.f);
}

void RenderSystem::Draw(Window* window, RenderFrame* renderer, Shape* shape)
{
	RenderShape(renderer->cmd, renderer->render_pass, shape, 400.f, 400.f);
}

RenderSystem* CreateRenderSystem(Window* window)
{
	return new RenderSystem(window);
}

#pragma endregion


#pragma region RenderFrame

void RenderFrame::InitFrame(Window* window)
{
	if (AcquireCmdAndRenderPass(window, &cmd, &render_pass))
		can_be_use = true;
	else
		can_be_use = false;
}

void RenderFrame::EndRenderPassAndSubmitCommand()
{
	SDL_EndGPURenderPass(render_pass);
	SDL_SubmitGPUCommandBuffer(cmd);

	render_pass = NULL;
	cmd = NULL;
	can_be_use = false;
}

RenderFrame* NewFrame(Window* window)
{
	RenderFrame* renderer = new RenderFrame();
	renderer->InitFrame(window);
	return renderer;
}

void EndAndSubmit(RenderFrame* renderer)
{
	renderer->EndRenderPassAndSubmitCommand();
	delete renderer;
}

#pragma endregion