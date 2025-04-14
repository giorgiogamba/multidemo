// Copyrigth 2025 Giorgio Gamba

#pragma once

#include "Pixel.h"

struct RenderTask
{
	RenderTask()
		: startLine(0)
		, endLine(0)
	{}

	Uint32 getPixelColor() const { return pixel.getColor(); }

	int startLine;

	int endLine;

	Pixel pixel;
};
