/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: 4
Created date: 2019/03/25
Term: Spring 2019
************************************************/
#include "Projection.h"

Affine CameraToWorld(const Camera& cam)
{
	return Affine(
		cam.Right(),
		cam.Up(),
		cam.Back(),
		cam.Eye()
	);
}

Affine WorldToCamera(const Camera& cam)
{
	return inverse(CameraToWorld(cam));
}

Matrix CameraToNDC(const Camera& cam)
{
	Matrix result;

	const Vector viewport = cam.ViewportGeometry();
	const float near = cam.NearDistance();
	const float far = cam.FarDistance();

	result[0] = Hcoord(2*viewport.z/viewport.x);
	result[1] = Hcoord(0.f, 2*viewport.z/viewport.y);
	result[2] = Hcoord(0.f, 0.f, (near+far)/(near-far), 2*near*far/(near-far));
	result[3] = Hcoord(0.f, 0.f, -1.f, 0.f);

	return result;
}