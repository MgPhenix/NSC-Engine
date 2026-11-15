#pragma once
#include <ToolsBox/Vector3.h>
#include <ToolsBox/Vector2.h>
#include "Color.h"

using Vector3f = Vector3<float>;
using UV	   = Vector2<float>;

struct Vertice
{
	Vector3f position;
	FColor  color;
	UV		 uv;

	Vertice(Vector3f pos, FColor color_f, UV uv) :
		position(pos),
		color(color_f),
		uv(uv)
	{}
};