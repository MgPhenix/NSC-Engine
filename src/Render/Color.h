#pragma once
#include <ToolsBox/PrimitiveTypes.h>

template<typename T>
struct Color
{
	T r;
	T g;
	T b;
	T a;

	Color(T red, T green, T blue, T alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)
	{}
};

using UColor = Color<uint8>;
using FColor = Color<float>;

FColor ConvertColor(const UColor& color);

UColor ConvertColor(const FColor& color);