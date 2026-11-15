#include "Sampler.h"


#include <SDL3/SDL.h>
#include <iostream>
#include "GPU.h"


SDL_GPUSampler* Sampler::GetSampler()
{
    return mp_sampler;
}

Sampler::Sampler()
{
//	CreateSampler();
}

Sampler::~Sampler()
{
	SDL_ReleaseGPUSampler(GPU::GetInstance().GetDevice(), mp_sampler);
}

bool Sampler::CreateSampler(bool nearest)
{
	SDL_GPUSamplerCreateInfo sampler_info = {};
	if (!nearest)
	{
		sampler_info.min_filter = SDL_GPU_FILTER_LINEAR; // Linear = melange les pixel voisins si pas sur un pixel exact -> mieux pour un rendu avec doux flou léger
		sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR; // Nearest = prend le pixel le plus proche -> mieux pour un rendu pixel art
		sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
	}
	else
	{
		sampler_info.min_filter = SDL_GPU_FILTER_NEAREST; // Linear = melange les pixel voisins si pas sur un pixel exact -> mieux pour un rendu avec doux flou léger
		sampler_info.mag_filter = SDL_GPU_FILTER_NEAREST; // Nearest = prend le pixel le plus proche -> mieux pour un rendu pixel art
		sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
	}

	sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE; //Clamp to edge repete le pixel de bord pas de repetition visible (je crois)
	sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE; // Repeat = repete la texture peut être + pour les tiles

	mp_sampler = SDL_CreateGPUSampler(GPU::GetInstance().GetDevice(), &sampler_info);

	if (mp_sampler == NULL)
	{
		std::cout << "Couldn't create Sampler : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}