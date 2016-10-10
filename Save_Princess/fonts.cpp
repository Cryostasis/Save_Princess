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

struct Symbol
{
	char C;
	int x, y;
	int width, height;
	ObjInfo obj;
};

GLuint fontTex = -1;
std::map<char, Symbol> fontMap;

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

		fontMap[buf.C] = buf;

		elem = elem->NextSiblingElement("char");
	}
	
	textCamera = Camera();

	delete[] name;
}

TextMesh::TextMesh(
	int wnd_w, int wnd_h, int X, int Y, char *text, vec4 color, float aspect, float scale)
{
	_color = color;
	_mesh.clear();
	_mesh.resize(strlen(text));
	int i = 0;
	GLfloat x = X * 2 - wnd_w + scale;
	GLfloat y = -Y * 2 + wnd_h - scale * aspect;
	//GLfloat x = X * 2 - wnd_w, y = -Y * 2 + wnd_h;
	//y += (GLfloat)fontMap[text[0]].height / 2 * scale / FONT_WIDTH_STD;
	y += scale * aspect / 2;
	while (text[i] != 0)
	{
		//x += (GLfloat)fontMap[text[i]].width / 2 * scale / FONT_WIDTH_STD;
		GLfloat chr_aspect = 
			(GLfloat)fontMap[text[i]].width / (GLfloat)fontMap[text[i]].height;
		_mesh[i] = Mesh(
			vec3((GLfloat)x / wnd_w, (GLfloat)y / wnd_h, 0),
			vec3(scale, scale, scale * aspect) / wnd_w,
			fontTex, &fontMap[text[i]].obj);
		_mesh[i].rotate(M_PI_2, 0, 0);
		x += chr_aspect * scale * aspect * 0.75;
		i++;
	}
}

void TextMesh::render(GLuint program, Camera &camera)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glUniform4fv(LINE_COLOR_LOC, 1, _color.v);

	glUniform1i(FONT_TEX, 0);
	for (size_t i = 0; i < _mesh.size(); i++)
		_mesh[i].render(program, camera, false);
	glEnable(GL_DEPTH_TEST);
}