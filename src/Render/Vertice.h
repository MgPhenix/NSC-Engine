#pragma once
#include <ToolsBox/ToolsBox.h>
#include "Color.h"

using UV = Vector2<float>;

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