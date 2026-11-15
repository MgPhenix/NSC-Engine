#pragma once

struct SDL_GPUSampler;


class Sampler
{
private:

	SDL_GPUSampler* mp_sampler = nullptr;

public:

	SDL_GPUSampler* GetSampler();

	Sampler();
	~Sampler();

	bool CreateSampler(bool nearest = false);
};