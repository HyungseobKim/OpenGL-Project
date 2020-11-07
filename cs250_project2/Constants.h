#ifndef CS250_CONSTANTS_H
#define CS250_CONSTANTS_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "Affine.h"

const Point O(0, 0, 0);
const float PI = 4.0f * atan(1.0f);
const float rotation_rate = 20.f;

// Axis unit vectors
const Vector EX(1, 0, 0),
			 EY(0, 1, 0),
			 EZ(0, 0, 1);

// Colors
const Vector WHITE(1, 1, 1),
RED(1, 0.21f, 0.153f),
GREEN(0, 1, 0),
YELLOW(1, 1, 0.3f),
PURPLE(1, 0, 1),
BLUE(0.12f, 0.392f, 0.9f),
GRAY(0.8f, 0.8f, 0.8f),
BLACK(0, 0, 0);

namespace Count
{
	const int Earth = 5;
	const int Moon = Earth * 3;
	const int Satellite = Moon;
	const int Width = 4;
	const int Height = 4;
}

namespace Origin
{
	const Point occluder = Point(-10, 3, 0);
	const Point camera = Point(0, 10, 10);
	const Point light = Point(0, 20, -10);
	const Point plane = Point(0, -0.5f, 0);
}

#endif