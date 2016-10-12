#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

#include "math/math3d.h"
#include "math/mathgl.h"

#include "mesh.h" 

class TextMesh
{
public:
	TextMesh(const int wnd_w, const int wnd_h, const int X, const int Y, const char *text, 
		const vec4 color, const float aspect, const float scale);
	void render(GLuint program, Camera &camera);
	void move_to(int x, int y);
private:
	GLfloat _scale;
	int _wndH, _wndW, _aspect;
	std::string _text;
	std::vector<Mesh> _mesh;
	vec4 _color;
};

void activate_font(char *file);