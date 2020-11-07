/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "WorldMeshR.h"

void WorldMeshR::SetMesh(Mesh& mesh_)
{
	mesh = &mesh_;
}

void WorldMeshR::Translate(Vector t)
{
	center = center + t;
}
void WorldMeshR::SetCenter(Point p)
{
	center = p;
}
void WorldMeshR::SetRotAxis(Vector axis)
{ 
	rot_axis = axis;
}
void WorldMeshR::SetRotRate(float rate)
{
	rot_rate = rate; 
}

void WorldMeshR::SetRotAngle(float angle)
{
	rot_angle = angle;
}

void WorldMeshR::SetRevCenter(Point p)
{
	rev_center = p;
}
void WorldMeshR::SetRevAxis(Vector axis)
{
	rev_axis = axis;
}
void WorldMeshR::SetRevRate(float rate)
{
	rev_rate = rate;
}

void WorldMeshR::SetRevRadius(float radius) 
{
	rev_radius = radius; 
}

void WorldMeshR::Rotate(double amount) 
{ 
	rot_angle += static_cast<float>(rot_rate * amount); 
	Clamp(rot_angle);
}
void WorldMeshR::Revolve(double amount)
{
	//-------------------- 1. Maintain radius of revolution --------------------
	if (rev_radius)
	{
		Vector direction = rev_center - center;
		const float distance = abs(direction) - rev_radius;

		direction = normalize(direction);
		for (int i = 0; i < 3; i++)
			direction[i] *= distance;

		center = translate(direction) * center;
	}

	//-------------------- 2. Make axis perpendicular --------------------
	const Vector unit_axis = normalize(rev_axis);
	const Vector unit_direction = normalize(center - rev_center);
	const Vector normal = cross(unit_direction, unit_axis);
	const float angle = std::acos(dot(unit_axis, unit_direction));
	rev_axis = rotate(PI / 2.f - angle, normal) * rev_axis;

	//-------------------- 3. Apply revolution to center --------------------
	center = translate(rev_center - O) *                                       // Third, return to position
		rotate(static_cast<float>(((rev_rate * amount) / 180.f)*PI), rev_axis) * // Second, revolve
		translate(O - rev_center) *                                            // First, translate with revolution center
		center;
}

void WorldMeshR::SetScale(Vector s)
{
	size.x += s.x*size_rate;
	size.y += s.y*size_rate;
	size.z += s.z*size_rate;
}
void WorldMeshR::SetSize(Vector s) 
{
	size = s;
}
void WorldMeshR::SetSizeRate(float rate) 
{
	size_rate = rate;
}

Mesh& WorldMeshR::GetMesh() 
{ 
	return *mesh;
}
Affine WorldMeshR::Get_Object_to_World() const
{
	return translate(center - O) * rotate((rot_angle / 180.f)*PI, rot_axis) * scale(size.x, size.y, size.z); 
}
Point WorldMeshR::GetCenter() const
{ 
	return center;
}

float WorldMeshR::GetRotAngle()
{ 
	return rot_angle;
}
Vector WorldMeshR::GetRotAxis()
{
	return rot_axis;
}

void WorldMeshR::Clamp(float& angle)
{
	while (angle > 0) angle -= 360.f;
}