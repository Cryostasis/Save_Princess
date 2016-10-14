#include "math/math3d.h"
#include "math/mathgl.h"
#include "tinyXML/tinyxml.h"

#include "fonts.h"
#include "error_log.h"
#include "textures.h"
#include "camera.h"
#include "mesh.h"
#include "obj_load.h"
#include "gl_vars.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <map>
#include <vector>
#include <iostream>
#pragma warning (disable:4996) //sprintf

using namespace std;

struct Symbol
{
	char C;
	int x, y;
	int width, height;
	ObjInfo obj;
	Mesh* mesh;
};

GLuint fontTex = -1;
map<char, Symbol> fontMap;

const int tex_size = 512;

#pragma warning (disable:4477) //sscanf

void activate_font(char *file)
{
	char *name = new char[100];
	name[0] = 0;
	sprintf(name, "%s.tga", file);
	
	fontTex = get_texture_from_tga(name);
	
	name[0] = 0;
	sprintf(name, "%s.xml", file);

	TiXmlDocument *doc = new TiXmlDocument(name);
	if (!doc->LoadFile())
		return;

	TiXmlElement *font = doc->FirstChildElement("font");
	TiXmlElement *elem = font->FirstChildElement("char");

	while (elem != NULL)
	{
		const char* id = elem->Attribute("id");
		const char* x = elem->Attribute("x");
		const char* y = elem->Attribute("y");
		const char* w = elem->Attribute("width");
		const char* h = elem->Attribute("height");
		Symbol buf;

		sscanf(id, "%d", &buf.C);
		sscanf(x, "%d", &buf.x);
		sscanf(y, "%d", &buf.y);
		sscanf(w, "%d", &buf.width);
		sscanf(h, "%d", &buf.height);

		buf.obj = clone_obj("objects/quad.obj");
		
		GLfloat x1 = GLfloat(buf.x) / tex_size, x2 = GLfloat(buf.x + buf.width) / tex_size;
		GLfloat y2 = GLfloat(buf.y) / tex_size, y1 = GLfloat(buf.y + buf.height) / tex_size;

		buf.obj.texcoords[0] = x1; buf.obj.texcoords[1] = y1;
		buf.obj.texcoords[2] = x2; buf.obj.texcoords[3] = y1;
		buf.obj.texcoords[4] = x2; buf.obj.texcoords[5] = y2;
		buf.obj.texcoords[6] = x1; buf.obj.texcoords[7] = y2;
		buf.obj.texcoords[8] = x1; buf.obj.texcoords[9] = y1;
		buf.obj.texcoords[10] = x2; buf.obj.texcoords[11] = y2;

		
		buf.mesh = new Mesh(
			vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), fontTex, &buf.obj);
		buf.mesh->rotate(M_PI_2, 0, 0);

		fontMap[buf.C] = buf;
		
		elem = elem->NextSiblingElement("char");
	}
	
	textCamera = Camera();

	delete[] name;
}

TextMesh::TextMesh(const int wnd_w, const int wnd_h, const int X, const int Y, const char *text,
	const vec4 color, const float aspect, const float scale)
{
	_text = text;
	_color = color;
	_wndH = wnd_h;
	_wndW = wnd_w;
	_aspect = aspect;
	_scale = scale;
	_x = X;
	_y = Y;
}

void TextMesh::move_to(int X, int Y)
{
	_x = X;
	_y = Y;
}

void TextMesh::set_text(string text)
{
	_text = text;
}

void TextMesh::render(GLuint program, Camera &camera)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glUniform4fv(LINE_COLOR_LOC, 1, _color.v);

	glUniform1i(FONT_TEX, 0);
	int i = 0;
	GLfloat x = _x * 2 - _wndW + _scale;
	GLfloat y = -_y * 2 + _wndH - _scale * _aspect;
	y += _scale * _aspect / 2;
	while (_text[i] != 0)
	{
		//x += (GLfloat)fontMap[text[i]].width / 2 * scale / FONT_WIDTH_STD;
		GLfloat chr_aspect =
			(GLfloat)fontMap[_text[i]].width / (GLfloat)fontMap[_text[i]].height;
		fontMap[_text[i]].mesh->set_size(vec3(_scale, _scale, _scale * _aspect) / _wndW);
		fontMap[_text[i]].mesh->move_to(vec3((GLfloat)x / _wndW, (GLfloat)y / _wndH, 0));
		
		fontMap[_text[i]].mesh->render(program, camera, false);

		x += chr_aspect * _scale / chr_aspect;
		i++;
	}

	glEnable(GL_DEPTH_TEST);
}


