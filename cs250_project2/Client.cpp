/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include <sstream>
#include <iostream>

#include "Client.h"

Client::Client(SDL_Window* win) : window(win)
{
	frame_count = 0;
	frame_time = 0;
	// Initialize renderer
	render = new Render();
	crender = new CameraRender(*render);
}


Client::~Client(void)
{
	// clean-up code:
	delete render;
	delete crender;
}


void Client::draw(double dt)
{
	//   dt: time (in seconds) since last animation frame
	// frame rate:
	++frame_count;
	frame_time += dt;
	if (frame_time >= 0.5)
	{
		double fps = frame_count / frame_time;
		frame_count = 0;
		frame_time = 0;
		std::stringstream ss;
		ss << "CS 250: Project #1 [fps=" << int(fps) << "]";
		SDL_SetWindowTitle(window, ss.str().c_str());
	}

	// -------------------- drawing code: --------------------
	render->ClearBuffers(WHITE);

	crender->SetCamera(cam);

	// All other objects
	world.Draw(crender, dt);

	time += dt;
}


void Client::MouseDown(const SDL_Event& event)
{
	auto button = event.button;
	world.MouseDown(ViewPort_to_World(static_cast<float>(button.x), static_cast<float>(button.y)));
}

void Client::MouseUp()
{
	world.MouseUp();
}

void Client::MouseMotion(const SDL_Event& event)
{
	auto button = event.button;
	world.HandleMouseMotion(ViewPort_to_World(static_cast<float>(button.x), static_cast<float>(button.y)));
}

Point Client::ViewPort_to_World(float x, float y)
{
	Hcoord mouse = Point(x, y, 1.f);

	Matrix3 vp_to_ndc;
	vp_to_ndc[0][0] = 2.f / width;
	vp_to_ndc[0][2] = static_cast<float>((1 - width)) / width;
	vp_to_ndc[1][1] = -2.f / height;
	vp_to_ndc[1][2] = static_cast<float>((height - 1)) / height;
	vp_to_ndc[2][2] = 1.f;

	mouse = vp_to_ndc * mouse;
	const Vector veiwport = cam.ViewportGeometry();

	Matrix3 ndc_to_vf;
	ndc_to_vf[0][0] = veiwport.x / 2.f;
	ndc_to_vf[1][1] = veiwport.y / 2.f;
	ndc_to_vf[2][2] = 1.f;

	mouse = ndc_to_vf * mouse;
	mouse.z = -cam.NearDistance();
	mouse.w = 1;

	const Vector u = cam.Right();
	const Vector v = cam.Up();
	const Vector n = cam.Back();
	const Point eye = cam.Eye();

	Matrix v_to_w;
	for (int i = 0; i < 4; i++)
	{
		v_to_w[i][0] = u[i];
		v_to_w[i][1] = v[i];
		v_to_w[i][2] = n[i];
		v_to_w[i][3] = eye[i];
	}

	mouse = v_to_w * mouse;

	return mouse;
}

void Client::resize(int W, int H)
{
	// respond to window resize
	//   W,H: window width and height (in pixels)
	glViewport(0, 0, W, H);

	width = W;
	height = H;
	aspect = static_cast<float>(W) / static_cast<float>(H);

	cam = Camera(
		Origin::camera, // Eye
		-EZ,            // look
		EY,             // relrative up
		2.f*PI / 3.f,   // fov
		aspect,         // aspect ratio
		0.01f,          // near
		100);           // far
}