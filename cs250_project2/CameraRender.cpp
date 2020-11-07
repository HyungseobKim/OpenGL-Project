/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include <vector>
#include <set>

#include "Constants.h"
#include "CameraRender.h"

CameraRender::CameraRender(Render& r) : render(r)
{
	SetCamera(Camera());
}

CameraRender::~CameraRender(void)
{
	world_vertices.clear();
	clip_vertices.clear();
}

void CameraRender::SetCamera(const Camera& cam)
{
	world2camera = WorldToCamera(cam);
	camera2ndc = CameraToNDC(cam);
}

void CameraRender::Draw(Mesh& mesh, const Affine& m, const Vector& color, const Point& light_source, const Hcoord& planeEquation)
{
	render.DrawMesh(mesh, camera2ndc * world2camera * m, color, inverse(m) * light_source);

	if (planeEquation != Hcoord())
		render.DrawShadow(mesh, m, camera2ndc * world2camera, light_source, planeEquation);
}

Matrix CameraRender::ProjectionToPlane(const Point& light_source, const Point& point, const Hcoord& plane)
{
	Matrix matrix;

	const Vector light = point - light_source;
	const float product = dot(Vector(plane.x, plane.y, plane.z), light);

	const float Lx = light.x;
	const float Ly = light.y;
	const float Lz = light.z;

	const float nx = plane.x;
	const float ny = plane.y;
	const float nz = plane.z;
	const float d = plane.w;

	matrix[0][0] = product + d - nx * Lx;
	matrix[0][1] = -ny * Lx;
	matrix[0][2] = -nz * Lx;
	matrix[0][3] = -Lx * d;

	matrix[1][0] = -nx * Ly;
	matrix[1][1] = product + d - ny * Ly;
	matrix[1][2] = -nz * Ly;
	matrix[1][3] = -Ly * d;

	matrix[2][0] = -nx * Lz;
	matrix[2][1] = -ny * Lz;
	matrix[2][2] = product + d - nz * Lz;
	matrix[2][3] = -Lz * d;

	matrix[3][0] = -nx;
	matrix[3][1] = -ny;
	matrix[3][2] = -nz;
	matrix[3][3] = product;

	return matrix;
}