#ifndef WORLD_H
#define WORLD_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "CubeMesh.h"
#include "SnubDodecMesh.h"

#include "WorldMesh.h"
#include "WorldMeshR.h"

namespace MeshType
{
	static CubeMesh cube;
	static SnubDodecMesh snub;
}

class CameraRender;

class World
{
public:
	World();

	void Draw(CameraRender* crender, double dt);

	void MouseDown(const Point& pos);
	void MouseUp();
	void HandleMouseMotion(const Point& pos);

private:
	void RayCast();

private:
	WorldMeshR light;
	WorldMesh occluder[4];
	WorldMesh plane;

	bool isMouseDown = false;
	Point mouse = Point();
	int picked = -1;
};

#endif