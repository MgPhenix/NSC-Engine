#pragma once
#include "RenderSystem.h"
#include "Window.h"
#include "Entity.h"


class Engine
{
private:

	RenderSystem m_render;
	Window		 m_window;


	bool m_running = true;

private:

	std::vector<Entity*> m_entitiesToDestroy;
	std::vector<Entity*> m_entitiesToAdd;
	std::vector<Entity*> m_entities;

public:

	void Init(const char* title, int width, int height);
	void Update();

	void AddEntity(Entity* entity);

	template<typename E, typename... Args>
	void CreateEntity(Args&&... args);

};


template<typename E, typename ...Args>
inline void Engine::CreateEntity(Args && ...args)
{
	static_assert(std::is_base_of_v<Entity, E>, "E must be derived from entity");

	E* entity = new E(args...);

	static_cast<Entity*>(entity)->Init();

	AddEntity(entity);
}