#pragma once

#define VERT_POSITION	0
#define VERT_TEXCOORD	1
#define VERT_NORMAL		2

#define FRAG_OUTPUT0	0

#define USE_TEX_LOC		5
#define LINE_COLOR_LOC  6
#define FONT_TEX		7

#include "camera.h"
#include "obj_load.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math/math3d.h"
#include "math/mathgl.h"

struct vertex_t
{
	GLfloat position[3];
	GLfloat texcoord[2];
	GLfloat normal[3];
};

class Mesh
{
public:
	Mesh() {};
	Mesh(vec3 pos, vec3 scale, GLuint tex, ObjInfo *mod);
	mat4 get_model_mat();
	void render(GLuint program, Camera &camera, bool regular);
	void render_normals(GLuint program, Camera &camera, bool regular);
	void render_pol_mesh(GLuint program, Camera &camera, bool regular);
	void render_with_norms(GLuint program, Camera &camera, bool regular);
	void rotate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(vec3 vec);
	void rotate_strict(GLfloat x, GLfloat y, GLfloat z);
	void rotate_strict(vec3 vec);
	void move(GLfloat x, GLfloat y, GLfloat z);
	void move(vec3 v);
	void move_to(GLfloat x, GLfloat y, GLfloat z);
	void move_to(vec3 v);
	void look_at(vec3 pos, vec3 target, vec3 up);
	void look_at_dir(vec3 pos, vec3 dir, vec3 up);
	void set_size(float sz);
	void set_size(vec3 sz);
protected:
	int material;
	GLuint VAO;
	GLuint VBO[4];
	GLuint N_VAO;
	GLuint N_VBO[2];
	GLuint P_VAO;
	GLuint P_VBO[2];
	int vert_cnt;
	int ind_cnt;
	vec3 position;
	vec3 rotation_angle;
	mat4 rotation;
	mat4 scale;
	vec3 scl;
	GLfloat size;
	GLuint texture;
	bool visible;
	mat4 aux_matrix;
	ObjInfo *model;
};

class FlatMesh : public Mesh
{
public:
	FlatMesh(const int wnd_w, const int wnd_h, const int x, const int y, const GLuint texture, 
		const float aspect, const float scale) :
		_wndWidth(wnd_w), _wndHeight(wnd_h), _scale(scale), _aspect(aspect), 
		Mesh(vec3(
				(GLfloat)(x * 2 - wnd_w + scale) / wnd_w,
				(GLfloat)(-y * 2 + wnd_h - scale) / wnd_h, 0),
			vec3(scale, scale, scale * aspect) / wnd_w, texture,
			clone_obj_ptr("objects/quad.obj")) { Mesh::rotate(M_PI_2, 0, 0); };
	void render(GLuint program, Camera &camera);
	void set_texture(GLuint tex);
	void move_to(int x, int y) 
	{
		Mesh::move_to((GLfloat)(x * 2 - _wndWidth + _scale) / _wndWidth,
			(GLfloat)(-y * 2 + _wndHeight - _scale) / _wndHeight, 0);
	};
private:
	int _wndWidth;
	int _wndHeight;
	GLfloat _scale;
	GLfloat _aspect;
};

extern GLuint norm_tex;
