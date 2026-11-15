#include "Entity.h"

void Entity::Init()
{
	OnInit();
}

void Entity::Update()
{
	for (Component* component : m_components)
		component->Update(this);

	OnUpdate();
}

void Entity::AddComponentToList(IComponent<Entity>* component)
{
	m_components.push_back(component);
}
