#pragma once
#include <ToolsBox/Vector2.h>
#include <ToolsBox/Serialization.h>
#include <ToolsBox/Traits.h>
#include <vector>
#include <SDL3/SDL.h>

#include "Component.h"

using Vector2f = Vector2<float>;

class Entity : public ITagSerializer
{
private:

	Vector2f m_position;

	std::vector<IComponent<Entity>*> m_components;

	bool m_released = false;

private:

	using Component = IComponent<Entity>;

public:

	Entity() = default;

	virtual void OnInit() {};
	virtual void OnUpdate() {};
	virtual void OnReset() {};
	virtual void OnDestroy() {};
	virtual void OnRelease() {};
	virtual void OnEvent(SDL_Event& event) {};

	void Reset();
	void Release();

	template<typename C, typename... Args>
	void AddComponent(Args&&... args);

	template<typename C>
	IComponent<Entity>* GetComponent();

private:

	void Init();
	void Update();

	void AddComponentToList(IComponent<Entity>* component);

	friend class Engine;
};


template<typename C, typename... Args>
inline void Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of_v<IComponent<Entity>, C>, "C must be derived from IComponent");

	C* comp = new C(args...);

	AddComponentToList(comp);
}

template<typename C>
inline IComponent<Entity>* Entity::GetComponent()
{
	for (auto& comp : m_components)
	{
		if (is_same_v<C, decltype(comp)>)
			return comp;
	}

	return nullptr;
}