// SnubDodecMesh.h
// -- mesh for a snub dodecahedron
// cs250 1/15

#ifndef CS250_SNUBDODEC_H
#define CS250_SNUBDODEC_H

#include "Mesh.h"


class SnubDodecMesh : public Mesh {
 public:
	 SnubDodecMesh();

	 int VertexCount(void);
	 Point GetVertex(int i);
	 Vector Dimensions(void);
	 Point Center(void);
	 int FaceCount(void);
	 Face GetFace(int i);
	 int EdgeCount(void);
	 Edge GetEdge(int i);

	 int GetType() override;

	 const Point* GetVertices() override;
	 const Face* GetIndices() override;
	 const Vector* GetNormal() override;

  private:
    static Point vertices[60];
    static Face faces[116];
    static Edge edges[150];

	Vector normal[180] = {};
};


#endif

