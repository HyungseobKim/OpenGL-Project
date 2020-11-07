/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: 3
Created date: 2019/03/20
Term: Spring 2019
************************************************/
#include "Camera.h"

Camera::Camera(void)
{
	eye = Point(0.f, 0.f, 0.f);

	back = Vector(0.f, 0.f, 1.f);
	up = Vector(0.f, 1.f, 0.f);
	right = Vector(1.f, 0.f, 0.f);

	near = 0.1f;
	far = 10.f;

	distance = near;
	width = distance * 2.f;
	height = width;
}

Camera::Camera(const Point& E, const Vector& look, const Vector& rel,
	float fov, float aspect, float near, float far)
{
	eye = E;

	back = -normalize(look);
	right = normalize(cross(look, rel));
	up = cross(back, right);

	this->near = near;
	this->far = far;

	distance = near;
	width = std::tan(fov/2.f) * 2.f * distance;
	height = width / aspect;
}

Point Camera::Eye(void) const
{
	return eye;
}

Vector Camera::Right(void) const
{
	return right;
}

Vector Camera::Up(void) const
{
	return up;
}

Vector Camera::Back(void) const
{
	return back;
}

Vector Camera::ViewportGeometry(void) const
{
	return Vector(width, height, distance);
}

float Camera::NearDistance(void) const
{
	return near;
}

float Camera::FarDistance(void) const
{
	return far;
}

Camera& Camera::Zoom(float factor)
{
	width *= factor;
	height *= factor;

	return *this;
}

Camera& Camera::Forward(float distance_increment)
{
	eye = eye - (distance_increment * back);

	return *this;
}

Camera& Camera::Yaw(float angle)
{
	const Affine rotation = rotate(angle, up);

	back = rotation * back;
	right = rotation * right;

	return *this;
}

Camera& Camera::Pitch(float angle)
{
	const Affine rotation = rotate(angle, right);

	back = rotation * back;
	up = rotation * up;

	return *this;
}

Camera& Camera::Roll(float angle)
{
	const Affine rotation = rotate(angle, back);

	right = rotation * right;
	up = rotation * up;

	return *this;
}