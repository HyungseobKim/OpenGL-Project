/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: 2
Created date: 2019/03/18
Term: Spring 2019
************************************************/
#include "CubeMesh.h"

const float CubeMesh::vertex_array[24] = {
	1.f, 1.f, 1.f,
	1.f, -1.f, 1.f,
	1.f, -1.f, -1.f,
	1.f, 1.f, -1.f,
	-1.f, 1.f, 1.f,
	-1.f, 1.f, -1.f,
	-1.f, -1.f, 1.f,
	-1.f, -1.f, -1.f
};

const unsigned int CubeMesh::indices[36] = {
	0, 1, 2,
	2, 3, 0,
	4, 0, 3,
	4, 3, 5,
	6, 4, 5,
	6, 5, 7,
	1, 6, 7,
	1, 7, 2,
	4, 6, 1,
	4, 1, 0,
	2, 7, 5,
	2, 5, 3
};

const Point CubeMesh::vertices[8] = {
	Point(1,1,1),
	Point(1,-1,1),
	Point(1,-1,-1),
	Point(1,1,-1),
	Point(-1,1,1),
	Point(-1,1,-1),
	Point(-1,-1,1),
	Point(-1,-1,-1)
};

const CubeMesh::Face CubeMesh::faces[12] = {
	Face(0,1,2),
	Face(2,3,0),
	Face(4,0,3),
	Face(4,3,5),
	Face(6,4,5),
	Face(6,5,7),
	Face(1,6,7),
	Face(1,7,2),
	Face(4,6,1),
	Face(4,1,0),
	Face(2,7,5),
	Face(2,5,3)
};

const CubeMesh::Edge CubeMesh::edges[12] = {
	Edge(0,1),
	Edge(1,2),
	Edge(2,3),
	Edge(3,0),
	Edge(3,5),
	Edge(5,4),
	Edge(4,0),
	Edge(6,4),
	Edge(6,7),
	Edge(7,5),
	Edge(6,1),
	Edge(7,2)
};

CubeMesh::CubeMesh()
{
	for (auto& face : faces)
	{
		Vector u = vertices[face.index2] - vertices[face.index1];
		Vector v = vertices[face.index3] - vertices[face.index2];
		Vector n = cross(u, v);
		
		normal[face.index1] = n + normal[face.index1];
		normal[face.index2] = n + normal[face.index2];
		normal[face.index3] = n + normal[face.index3];
	}
}

int CubeMesh::GetType()
{
	return 0;
}

const Point* CubeMesh::GetVertices()
{
	return vertices;
}

const CubeMesh::Face* CubeMesh::GetIndices()
{
	return faces;
}

const Vector* CubeMesh::GetNormal()
{
	return normal;
}

int CubeMesh::VertexCount(void)
{
	return 8;
}

Point CubeMesh::GetVertex(int i)
{
	return vertices[i];
}

Vector CubeMesh::Dimensions(void)
{
	return Vector(2, 2, 2);
}

Point CubeMesh::Center(void)
{
	return Point(0, 0, 0);
}

int CubeMesh::FaceCount(void)
{
	return 12;
}

CubeMesh::Face CubeMesh::GetFace(int i)
{
	return faces[i];
}

int CubeMesh::EdgeCount(void)
{
	return 12;
}

CubeMesh::Edge CubeMesh::GetEdge(int i)
{
	return edges[i];
}