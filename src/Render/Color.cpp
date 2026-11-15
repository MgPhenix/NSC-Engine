#include "Color.h"

FColor ConvertColor(const UColor& color)
{
	return FColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

UColor ConvertColor(const FColor& color)
{
	return UColor(static_cast<int>(color.r * 255), static_cast<int>(color.g * 255), static_cast<int>(color.b * 255), static_cast<int>(color.a * 255));
}
