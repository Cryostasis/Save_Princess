#include "math/math3d.h"
#include "math/mathgl.h"

#include "mesh.h"
#include "glerrors.h"

#include <math.h>

#define _USE_MATH_DEFINES

GLuint norm_tex;

Mesh::Mesh(vec3 pos, vec3 scale, GLuint tex, ObjInfo *mod)
{
	vert_cnt = mod->vert_num;
	ind_cnt = mod->ind_num;

	position = pos;
	rotation = mat4_identity;
	rotation_angle = vec3(0, 0, 0);
	this->scale = GLScale(scale);
	size = fmax(fmax(scale.v[0], scale.v[1]), scale.v[2]);
	texture = tex;
	visible = true;
	physics = false;
	model = mod;
	aux_matrix = mat4_identity;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(4, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vert_cnt * sizeof(GLfloat) * 3, &model->verts[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(VERT_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vert_cnt * sizeof(GLfloat) * 2, &model->texcoords[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, vert_cnt * sizeof(GLfloat) * 3, &model->normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_NORMAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt * sizeof(GLuint), &model->indicies[0], GL_STATIC_DRAW);


	glGenVertexArrays(1, &N_VAO);
	glBindVertexArray(N_VAO);

	glGenBuffers(2, N_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, N_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vert_cnt * 2 * sizeof(GLfloat) * 3, &model->N_verts[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(VERT_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_POSITION);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, N_VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt * 2 * sizeof(GLuint), &model->N_indicies[0], GL_STATIC_DRAW);


	glGenVertexArrays(1, &P_VAO);
	glBindVertexArray(P_VAO);

	glGenBuffers(2, P_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, P_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vert_cnt * 2 * sizeof(GLfloat) * 3, &model->P_verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(VERT_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_POSITION);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P_VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_cnt * 2 * sizeof(GLuint), &model->P_indicies[0], GL_STATIC_DRAW);

	check_GL_error();
}

mat4 Mesh::get_model_mat()
{
	return aux_matrix * GLTranslation(position) * rotation * scale;
}

void Mesh::render(GLuint program, Camera &camera, bool regular)
{
	camera.setup(program, get_model_mat());

	if (regular)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ind_cnt, GL_UNSIGNED_INT, NULL);
}

void Mesh::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	rotation_angle += vec3(x, y, z);
	rotation = GLrotate(rotation_angle);
}

void Mesh::rotate(vec3 vec)
{
	rotate(vec.v[0], vec.v[1], vec.v[2]);
}

void Mesh::rotate_strict(GLfloat x, GLfloat y, GLfloat z)
{
	rotation_angle = vec3(x, y, z);
	rotation = GLrotate(rotation_angle);
}

void Mesh::rotate_strict(vec3 vec)
{
	rotate_strict(vec.v[0], vec.v[1], vec.v[2]);
}

void Mesh::move(GLfloat x, GLfloat y, GLfloat z)
{
	position = position + vec3(x, y, z);
}

void Mesh::move(vec3 v)
{
	position = position + v;
}

void Mesh::move_to(GLfloat x, GLfloat y, GLfloat z)
{
	position = vec3(x, y, z);
}

void Mesh::move_to(vec3 v)
{
	position = v;
}

void Mesh::set_size(float sz)
{
	size = sz;
	scale = GLScale(sz, sz, sz);
}