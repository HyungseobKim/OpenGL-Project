#ifndef CS250_WORLDMESH_H
#define CS250_WORLDMESH_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "Mesh.h"
#include "Constants.h"

class WorldMesh
{
public:
	void SetMesh(Mesh& mesh_);

	void Translate(Vector t);
	void SetCenter(Point p);
	void SetScale(Vector s);
	void SetSize(Vector s);
	void SetSizeRate(float rate);

	Mesh& GetMesh();
	Affine Get_Object_to_World() const;
	Point GetCenter() const;

private:
	Mesh* mesh;
	Point center = O;

	Vector size = Vector(1.f, 1.f, 1.f);
	float size_rate = 0.f;
};

#endif