#ifndef CS250_CLIENT_H
#define CS250_CLIENT_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include <SDL2/SDL.h>

#include "Constants.h"
#include "CameraRender.h"

#include "World.h"

class Client
{
public:
	Client(SDL_Window* window);
	~Client(void);
	void draw(double dt);
	void MouseDown(const SDL_Event& event);
	void MouseUp();
	void MouseMotion(const SDL_Event& event);
	void resize(int W, int H);

private:
	Point ViewPort_to_World(float x, float y);

private:
	// variables for frame rate:
	SDL_Window* window;
	int frame_count;
	double frame_time;

	// other variables:
	double time = 0;
	int width = 600;
	int height = 600;
	float aspect = 1.f;

	World world;

	// Camera
	Camera cam = Camera(
		Origin::camera, // Eye
		-EZ,            // look
		EY,             // relrative up
		2.f*PI / 3.f,   // fov
		1,              // aspect ratio
		0.01f,          // near
		100);           // far;

	// Renderer
	Render* render;
	CameraRender* crender;
};

#endif