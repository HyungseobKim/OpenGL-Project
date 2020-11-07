// Render.h
// -- simple rendering using OpenGL
// cs250 1/17

#ifndef CS250_RENDER_H
#define CS250_RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"

class Render {
public:
    Render(void);
    ~Render(void);

    void ClearBuffers(const Vector& c);

	void DrawMesh(Mesh& mesh, const Matrix& m, const Vector& color, const Point& light_source);
	void DrawShadow(Mesh& mesh, const Affine& model_to_world, const Matrix& world_to_ndc, const Point& light_source, const Hcoord& planeEquation);

private:
	void CompileShader();
	void CompileShadowShader();

private:
	GLint program, shadow_program;
	GLint ucolor, utransform, umodel2world, uworld2ndc, uplane;
	GLint ulight_source[2];
};


#endif

