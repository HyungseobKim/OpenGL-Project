#ifndef CS250_CAMERARENDER_H
#define CS250_CAMERARENDER_H
/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: project 2
Created date: 2019/05/29
Term: Spring 2019
************************************************/
#include <vector>
#include <set>

#include "Mesh.h"
#include "Render.h"
#include "Projection.h"

class CameraRender {
public:
	CameraRender(Render& r);
	~CameraRender(void);
	void SetCamera(const Camera& cam);
	void Draw(Mesh& mesh, const Affine& m, const Vector& color, const Point& light_source, const Hcoord& planeEquation = Hcoord());
	
private:
	Matrix ProjectionToPlane(const Point& light_source, const Point& point, const Hcoord& plane);

private:
	Render& render;
	Affine world2camera;
	Matrix camera2ndc;
	std::vector<Point> world_vertices;
	std::vector<Point> camera_vertices;
	std::vector<Hcoord> clip_vertices;
};

#endif