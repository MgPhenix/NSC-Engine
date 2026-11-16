#pragma once


template<typename T>
struct IComponent
{
	virtual void Update(T* owner) = 0;

	using Type = IComponent<T>;
};