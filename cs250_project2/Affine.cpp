/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: 1
Created date: 2019/03/13
Term: Spring 2019
************************************************/
#include "Affine.h"

// Helper functions
float operator*(const Hcoord& u, const Hcoord& v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
}

Hcoord GetColumn(const Matrix& matrix, int column)
{
	assert(column < 4);

	return Hcoord(matrix[0][column], matrix[1][column], matrix[2][column], matrix[3][column]);
}

Matrix operator+(const Matrix& A, const Matrix& B)
{
	Matrix result;

	for (int i = 0; i < 4; ++i)
		result[i] = A[i] + B[i];

	return result;
}

Matrix operator*(float r, const Matrix& matrix)
{
	Matrix result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = r * matrix[i][j];

	return result;
}

Matrix ConstructMatrix(const Vector& u, const Vector& v)
{
	Matrix result;

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			result[i][j] = u[i] * v[j];

	return result;
}

Matrix transpose(const Matrix& matrix)
{
	Matrix result;

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			result[i][j] = matrix[j][i];

	return result;
}

bool operator==(const Hcoord& a, const Hcoord& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

bool operator!=(const Hcoord& a, const Hcoord& b)
{
	return !(a == b);
}

// Homework
Hcoord::Hcoord(float X, float Y, float Z, float W)
	: x(X), y(Y), z(Z), w(W)
{
}

Point::Point(float X, float Y, float Z)
	: Hcoord(X, Y, Z, 1)
{
}

Vector::Vector(float X, float Y, float Z)
	: Hcoord(X, Y, Z, 0)
{
}

Affine::Affine(void)
{
	row[3].w = 1.f;
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	for (int i = 0; i < 3; ++i)
	{
		row[i].x = Lx[i];
		row[i].y = Ly[i];
		row[i].z = Lz[i];
		row[i].w = D[i];
	}

	row[3].w = D.w;
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	return Hcoord(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	return Hcoord(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

Hcoord operator-(const Hcoord& v)
{
	return Hcoord(-v.x, -v.y, -v.z, -v.w);
}

Hcoord operator*(float r, const Hcoord& v)
{
	return Hcoord(r*v.x, r*v.y, r*v.z, r*v.w);
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; ++i)
		result[i] = A[i] * v;

	return result;
}

Hcoord operator*(const Matrix3& M, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 3; i++)
	{
		Hcoord row = M[i];
		result[i] = row[0] * v[0] + row[1] * v[1] + row[2] * v[2];
	}

	return result;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = A[i] * GetColumn(B, j);

	return result;
}

float dot(const Vector& u, const Vector& v)
{
	return u * v;
}

float abs(const Vector& v)
{
	return std::sqrt(dot(v, v));
}

Vector normalize(const Vector& v)
{
	return (1/abs(v)) * v;
}

Vector cross(const Vector& u, const Vector& v)
{
	Vector result;

	result.x = u.y*v.z - u.z*v.y;
	result.y = u.z*v.x - u.x*v.z;
	result.z = u.x*v.y - u.y*v.x;

	return result;
}

Affine rotate(float t, const Vector& v)
{
	// First Matrix
	Matrix result;

	const float cosin = std::cos(t);

	for (int i = 0; i < 3; ++i)
		result[i][i] = cosin;

	// Second Matrix
	Vector axis = normalize(v);

	result = result + ((1 - cosin) * ConstructMatrix(axis, axis));

	// Third Matrix
	Matrix temp;

	temp[0][1] = -axis.z;
	temp[0][2] = axis.y;

	temp[1][0] = axis.z;
	temp[1][2] = -axis.x;

	temp[2][0] = -axis.y;
	temp[2][1] = axis.x;

	temp = std::sin(t) * temp;

	// Add matrix
	result = result + temp;
	result[3][3] = 1;

	return result;
}

Affine translate(const Vector& v)
{
	Affine result;

	for (int i = 0; i < 4; ++i)
		result[i][i] = 1;

	result[0].w = v.x;
	result[1].w = v.y;
	result[2].w = v.z;

	return result;
}

Affine scale(float r)
{
	Matrix result;

	for (int i = 0; i < 4; ++i)
		result[i][i] = 1;

	result = r * result;
	result[3][3] = 1;

	return result;
}

Affine scale(float rx, float ry, float rz)
{
	return Affine(
		Vector(rx, 0, 0),
		Vector(0, ry, 0),
		Vector(0, 0, rz),
		Point()
	);
}

Affine inverse(const Affine& A)
{
	Matrix cofactor;

	// Construct determinant matrix
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			cofactor[i][j] = A[(i + 1) % 3][(j + 1) % 3] * A[(i + 2) % 3][(j + 2) % 3] - A[(i + 2) % 3][(j + 1) % 3] * A[(i + 1) % 3][(j + 2) % 3];

	const float determinant = A[0] * cofactor[0];

	Matrix translation;

	for (int i = 0; i < 3; ++i)
	{
		translation[i][i] = 1;
		translation[i].w = -A[i].w;
	}

	Matrix result = ((1 / determinant) * transpose(cofactor)) * translation;
	result[3][3] = 1;

	return result;
}