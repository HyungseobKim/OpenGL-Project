/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include "WorldMesh.h"

void WorldMesh::SetMesh(Mesh& mesh_)
{
	mesh = &mesh_; 
}

void WorldMesh::Translate(Vector t)
{
	center = center + t;
}

void WorldMesh::SetCenter(Point p)
{
	center = p;
}
void WorldMesh::SetScale(Vector s)
{
	size.x += s.x*size_rate;
	size.y += s.y*size_rate;
	size.z += s.z*size_rate;
}
void WorldMesh::SetSize(Vector s)
{ 
	size = s; 
}
void WorldMesh::SetSizeRate(float rate) 
{ 
	size_rate = rate; 
}

Mesh& WorldMesh::GetMesh() 
{
	return *mesh;
}
Affine WorldMesh::Get_Object_to_World() const
{ 
	return translate(center - O) * scale(size.x, size.y, size.z);
}
Point WorldMesh::GetCenter() const 
{
	return center;
}