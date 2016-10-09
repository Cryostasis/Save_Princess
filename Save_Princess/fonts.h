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
	TextMesh(int wnd_w, int wnd_h, int X, int Y, char *text, vec4 color, float aspect, float scale);
	void render(GLuint program, Camera &camera);
private:

	std::vector<Mesh> _mesh;
	vec4 _color;
};

void activate_font(char *file);