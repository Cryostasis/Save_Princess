#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <string>

#include "math/math3d.h"
#include "math/mathgl.h"

#include "mesh.h" 

using namespace std;

class TextMesh
{
public:
	TextMesh(const int wnd_w, const int wnd_h, const int X, const int Y, const char *text, 
		const vec4 color, const float aspect, const float scale);
	void render(GLuint program, Camera &camera);
	void move_to(int x, int y);
	void set_text(string text);
private:
	GLfloat _scale;
	int _wndH, _wndW, _aspect;
	std::string _text;
	//std::vector<Mesh> _mesh;
	vec4 _color;
	int _x;
	int _y;
};

void activate_font(char *file);