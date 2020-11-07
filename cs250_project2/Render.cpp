/************************************************
Author: Kim Hyungseob
Course: CS250
Homework: 2
Created date: 2019/03/18
Term: Spring 2019
************************************************/
#include <stdexcept>

#include "CubeMesh.h"
#include "SnubDodecMesh.h"

#include "Render.h"

namespace
{
	GLuint VAO[2] = {};
	GLuint VBO[2] = {};
	GLuint EBO[2] = {};

	constexpr int log_length = 512;
}

Render::Render(void)
{
	// Compile shaders
	CompileShader();
	CompileShadowShader();

	// Set attributes
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	CubeMesh cube;

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Point) + sizeof(Vector)) * cube.VertexCount(), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * cube.VertexCount(), cube.GetVertices());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Point) * cube.VertexCount(), sizeof(Vector) * cube.VertexCount(), cube.GetNormal());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(0));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vector), reinterpret_cast<void*>(sizeof(Point) * cube.VertexCount()));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * cube.FaceCount(), cube.GetIndices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SnubDodecMesh snub;

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Point) + sizeof(Vector)) * snub.VertexCount(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * snub.VertexCount(), snub.GetVertices());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Point) * snub.VertexCount(), sizeof(Vector) * snub.VertexCount(), snub.GetNormal());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vector), reinterpret_cast<void*>(sizeof(Point) * snub.VertexCount()));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * snub.FaceCount(), snub.GetIndices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::CompileShader()
{
	int success;
	char info_log[log_length];

	// Compile vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertex_shader_source = R"(
		#version 400
		layout(location = 0) in vec4 position;
		layout(location = 1) in vec4 normal;

		out vec3 frag_color;
		
		uniform vec3 color;
		uniform mat4 transform;
		uniform vec4 light_source;

		void main() {
			gl_Position = transform * position;
			vec4 light = light_source - position;
			float mu = dot(light, normal) / (length(normal) * length(light));
			if (mu < 0) mu = 0;
			frag_color = mu * color;
		})";

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		throw(std::runtime_error(info_log));
	}

	// Compile fragment shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragment_shader_source = R"(
		#version 400
		in vec3 frag_color;
		void main(void) {
			gl_FragColor = vec4(frag_color, 1.0);
		})";

	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw(std::runtime_error(info_log));
	}

	// Link shaders
	program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(program, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glDeleteProgram(program);
		throw(std::runtime_error(info_log));
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(program);

	ucolor = glGetUniformLocation(program, "color");
	utransform = glGetUniformLocation(program, "transform");
	ulight_source[0] = glGetUniformLocation(program, "light_source");
}

void Render::CompileShadowShader()
{
	int success;
	char info_log[log_length];

	// Compile vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertex_shader_source = R"(
		#version 400
		layout(location = 0) in vec4 position;

		uniform mat4 model2world;
		uniform mat4 world2ndc;
		uniform vec4 light_source;
		uniform vec4 plane;

		void main() {
			vec4 pos = model2world * position;
			vec4 light = pos - light_source;

			float Lx = light.x;
			float Ly = light.y;
			float Lz = light.z;

			float nx = plane.x;
			float ny = plane.y;
			float nz = plane.z;
			float d = plane.w;

			float product = dot(vec4(nx, ny, nz, 0.0), light);

			mat4 projection;
			projection[0] = vec4(product + d - nx * Lx, -nx * Ly, -nx * Lz, -nx);
			projection[1] = vec4(-ny * Lx, product + d - ny * Ly, -ny * Lz, -ny);
			projection[2] = vec4(-nz * Lx, -nz * Ly, product + d - nz * Lz, -nz);
			projection[3] = vec4(-Lx * d, -Ly * d, -Lz * d, product);

			pos = projection * pos;
			pos = (1 / pos.w) * pos;

			gl_Position = world2ndc * pos;
		})";

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		throw(std::runtime_error(info_log));
	}

	// Compile fragment shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragment_shader_source = R"(
		#version 400

		void main(void) {
			gl_FragColor = vec4(0.15, 0.15, 0.15, 1.0);
		})";

	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		throw(std::runtime_error(info_log));
	}

	// Link shaders
	shadow_program = glCreateProgram();

	glAttachShader(shadow_program, vertex_shader);
	glAttachShader(shadow_program, fragment_shader);
	glLinkProgram(shadow_program);

	glGetShaderiv(shadow_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shadow_program, log_length, NULL, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glDeleteProgram(shadow_program);
		throw(std::runtime_error(info_log));
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(shadow_program);

	umodel2world = glGetUniformLocation(shadow_program, "model2world");
	uworld2ndc = glGetUniformLocation(shadow_program, "world2ndc");
	uplane = glGetUniformLocation(shadow_program, "plane");
	ulight_source[1] = glGetUniformLocation(shadow_program, "light_source");
}

Render::~Render(void)
{
	glDeleteProgram(program);
	glUseProgram(0);

	for (int i = 0; i < 4; i++)
		glDeleteBuffers(i, VBO);
}

void Render::ClearBuffers(const Vector& c)
{
	glClearColor(c.x, c.y, c.z, c.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::DrawMesh(Mesh& mesh, const Matrix& m, const Vector& color, const Point& light_source)
{
	glUseProgram(program);

	glBindVertexArray(VAO[mesh.GetType()]);

	float c[3] = { color.x, color.y, color.z };
	glUniform3fv(ucolor, 1, c);

	Matrix matrix = m;
	glUniformMatrix4fv(utransform, 1, GL_TRUE, &matrix[0][0]);

	float light[4] = { light_source.x, light_source.y, light_source.z, light_source.w };
	glUniform4fv(ulight_source[0], 1, light);

	glDrawElements(GL_TRIANGLES, mesh.FaceCount() * 3, GL_UNSIGNED_INT, 0);
}

void Render::DrawShadow(Mesh& mesh, const Affine& model_to_world, const Matrix& world_to_ndc, const Point& light_source, const Hcoord& planeEquation)
{
	glUseProgram(shadow_program);

	glBindVertexArray(VAO[mesh.GetType()]);

	Affine model2world = model_to_world;
	glUniformMatrix4fv(umodel2world, 1, GL_TRUE, &model2world[0][0]);

	Matrix world2ndc = world_to_ndc;
	glUniformMatrix4fv(uworld2ndc, 1, GL_TRUE, &world2ndc[0][0]);

	float plane[4] = { planeEquation.x, planeEquation.y, planeEquation.z, planeEquation.w };
	glUniform4fv(uplane, 1, plane);

	float light[4] = { light_source.x, light_source.y, light_source.z, light_source.w };
	glUniform4fv(ulight_source[1], 1, light);

	glDrawElements(GL_TRIANGLES, mesh.FaceCount() * 3, GL_UNSIGNED_INT, 0);
}