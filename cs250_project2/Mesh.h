// Mesh.h
// -- 3D triangular mesh interface
// cs250 9/16

#ifndef CS250_MESH_H
#define CS250_MESH_H

#include "Affine.h"

struct Mesh {

  struct Face {
    unsigned int index1, index2, index3;
	Face(unsigned int i = -1, unsigned int j = -1, unsigned int k = -1)
		: index1(i), index2(j), index3(k) {}
  };

  struct Edge {
    int index1, index2;
	Edge(int i = -1, int j = -1)
		: index1(i), index2(j) {}
  };

  virtual ~Mesh(void) {}
  virtual int VertexCount(void) = 0;
  virtual Point GetVertex(int i) = 0;
  virtual Vector Dimensions(void) = 0;
  virtual Point Center(void) = 0;
  virtual int FaceCount(void) = 0;
  virtual Face GetFace(int i) = 0;
  virtual int EdgeCount(void) = 0;
  virtual Edge GetEdge(int i) = 0;

  virtual int GetType() = 0;

  virtual const Point* GetVertices() = 0;
  virtual const Face* GetIndices() = 0;
  virtual const Vector* GetNormal() = 0;
};


#endif

