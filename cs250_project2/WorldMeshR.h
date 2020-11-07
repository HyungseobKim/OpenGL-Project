#ifndef CS250_WORLDMESHR_H
#define CS250_WORLDMESHR_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "Affine.h"
#include "Mesh.h"
#include "Constants.h"

class WorldMeshR
{
public:
	void SetMesh(Mesh& mesh_);

	void Translate(Vector t);
	void SetCenter(Point p);
	void SetRotAxis(Vector axis);
	void SetRotRate(float rate);

	void SetRotAngle(float angle);

	void SetRevCenter(Point p);
	void SetRevAxis(Vector axis);
	void SetRevRate(float rate);

	void SetRevRadius(float radius);

	void Rotate(double amount);
	void Revolve(double amount);

	void SetScale(Vector s);
	void SetSize(Vector s);
	void SetSizeRate(float rate);

	Mesh& GetMesh();
	Affine Get_Object_to_World() const;
	Point GetCenter() const;

	float GetRotAngle();
	Vector GetRotAxis();

private:
	Mesh* mesh;

	Point center = O;
	Vector rot_axis = EX;
	float rot_rate = 0.f;
	float rot_angle = 0.f;

	Point rev_center = O;
	Vector rev_axis = EX;
	float rev_rate = 0.f;
	float rev_radius = 0.f;

	Vector size = Vector(1.f, 1.f, 1.f);
	float size_rate = 0.f;

private:
	void Clamp(float& angle);
};

#endif