// Copyright 2025 Giorgio Gamba

#pragma once

#include "SDL3/SDL_render.h"

// This struct represents how a pixel is intended inside the texture drawn
struct Pixel
{

public:

	Pixel()
		: r(0)
		, g(0)
		, b(0)
	{}

	Pixel(const int inR, const int inG, const int inB)
		: r(inR)
		, g(inG)
		, b(inB)
	{
		const Uint8 red = static_cast<Uint8>(r);
		const Uint8 green = static_cast<Uint8>(g);
		const Uint8 blue = static_cast<Uint8>(b);

		constexpr int a = 255;
		color = (a << 24) | (red << 16) | (green << 8) | blue;
	}

	Uint32 getColor() const { return color; }

private:

	Uint32 color;

	// The three quantities representing the pixel color
	int r, g, b;

	// Alpha is assumed to always be at max value (255 in 8 bits)
};
