#include "Pipeline.h"


#include <SDL3/SDL.h>
#include <iostream>
#include <GPU.h>


Pipeline::Pipeline(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader)
{
	CreatePipeline(window, device, vertex_shader, fragment_shader);
}

Pipeline::~Pipeline()
{
	//if (mp_pipeline != NULL)
	//{
	//	std::cout << "You didn't release the pipeline before, it can cause problem or memory leak" << std::endl;
	//}
}

SDL_GPUGraphicsPipeline* Pipeline::GetPipeline()
{
    return mp_pipeline;
}

bool Pipeline::CreatePipeline(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader)
{
	SDL_GPUVertexBufferDescription vertex_buffer_desc = {};
	vertex_buffer_desc.slot = 0;
	vertex_buffer_desc.pitch = sizeof(float) * 9; // 3 float de position + 4 float de couleur
	vertex_buffer_desc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
	vertex_buffer_desc.instance_step_rate = 0;


	SDL_GPUVertexAttribute attributes[3];
	attributes[0].location = 0;
	attributes[0].buffer_slot = 0;
	attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
	attributes[0].offset = 0;

	attributes[1].location = 1;
	attributes[1].buffer_slot = 0;
	attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
	attributes[1].offset = sizeof(float) * 3; // Ont est apres les 3 float de position

	attributes[2].location = 2;
	attributes[2].buffer_slot = 0;
	attributes[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
	attributes[2].offset = sizeof(float) * 7; //apres les 3 float de pos et les 4 float de couleur

	SDL_GPUVertexInputState vertex_input_state = {};
	vertex_input_state.vertex_buffer_descriptions = &vertex_buffer_desc;
	vertex_input_state.vertex_attributes = attributes;
	vertex_input_state.num_vertex_attributes = 3; // Position et color et UV
	vertex_input_state.num_vertex_buffers = 1; // un seul buffer de data


	SDL_GPURasterizerState rasterizer_state = {};
	rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL; //Remplie le triangle, Line mode = ne dessine que les contours
	rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE; // Désactive le culling;
	rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE; // Sert quand Culling est activé pour définir quel face est la face avant


	SDL_GPUColorTargetBlendState blend_state = {};
	blend_state.enable_blend = true;
	blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
	blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
	blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;

	SDL_GPUColorTargetDescription color_target_desc = {};
	color_target_desc.format = SDL_GetGPUSwapchainTextureFormat(device, window);
	color_target_desc.blend_state = blend_state;


	SDL_GPUGraphicsPipelineTargetInfo target_info = {};
	target_info.color_target_descriptions = &color_target_desc;
	target_info.num_color_targets = 1;


	SDL_GPUGraphicsPipelineCreateInfo info = {};
	info.vertex_shader = vertex_shader;
	info.fragment_shader = fragment_shader;
	info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	info.vertex_input_state = vertex_input_state;
	info.rasterizer_state = rasterizer_state;
	info.target_info = target_info;

	mp_pipeline = SDL_CreateGPUGraphicsPipeline(device, &info);
	if (mp_pipeline == NULL)
	{
		std::cout << "Can't create pipeline : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Pipeline::Destroy()
{
	SDL_ReleaseGPUGraphicsPipeline(GPU::GetInstance().GetDevice(), mp_pipeline);
}