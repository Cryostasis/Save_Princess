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
	void rotate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(vec3 vec);
	void rotate_strict(GLfloat x, GLfloat y, GLfloat z);
	void rotate_strict(vec3 vec);
	void move(GLfloat x, GLfloat y, GLfloat z);
	void move(vec3 v);
	void move_to(GLfloat x, GLfloat y, GLfloat z);
	void move_to(vec3 v);
	void set_size(float sz);
private:
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
	int material;
	GLuint texture;
	bool visible;
	bool physics;
	mat4 aux_matrix;
	ObjInfo *model;
};

extern GLuint norm_tex;