// CubeMesh.h
// -- standard cube
// cs250 5/15

#ifndef CS250_CUBEMESH_H
#define CS250_CUBEMESH_H

#include "Mesh.h"

class CubeMesh : public Mesh {
  public:
	  CubeMesh();

    int VertexCount(void) override;
    Point GetVertex(int i) override;
    Vector Dimensions(void) override;
    Point Center(void) override;
    int FaceCount(void) override;
    Face GetFace(int i) override;
    int EdgeCount(void) override;
    Edge GetEdge(int i) override;

	int GetType() override;

	const Point* GetVertices() override;
	const Face* GetIndices() override;
	const Vector* GetNormal() override;

  private:
    static const Point vertices[8];
    static const Face faces[12];
    static const Edge edges[12];

	static const float vertex_array[24];
	static const unsigned int indices[36];
	Vector normal[24] = {};
};

#endif

