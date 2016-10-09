#include "math/math3d.h"
#include "math/mathgl.h"
#include "camera.h"
#include "gl_vars.h"
#define _USE_MATH_DEFINES
#include <cmath>

Camera::Camera()
{
	position = vec3(0, 0, 0);
	projection = mat4_identity;
	rotation = vec3(0, 0, 0);
}

Camera::Camera(float x, float y, float z)
{
	position = vec3(x, y, z);
	projection = mat4_identity;
	rotation = vec3(0, 0, 0);
}

void Camera::calc_perspective(float fov, float aspect, float zNear, float zFar)
{
	projection = GLPerspective(fov, aspect, zNear, zFar);
}

void Camera::calc_orto(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projection = GLOrthographic(left, right, bottom, top, zNear, zFar);
}

void Camera::rotate(float x, float y, float z)
{
	rotation.v[0] = rotation.v[0] + x;
	rotation.v[1] = rotation.v[1] + y;

	if (rotation.v[0] < -M_PI / 2)
		rotation.v[0] = -M_PI / 2;
	if (rotation.v[0] > M_PI / 2)
		rotation.v[0] = M_PI / 2;
}

void Camera::move_to(float x, float y, float z)
{
	position = vec3(x, y, z);
}

void Camera::move_to(vec3 vec)
{
	move_to(vec.v[0], vec.v[1], vec.v[2]);
}

void Camera::move(float x, float y, float z)
{
	vec4 buf = transpose(GLrotate(rotation)) * vec4(x, y, z, 1.0);
	vec3 move = vec3(buf.v[0], buf.v[1], buf.v[2]);

	position += move;
}

void Camera::look_at(const vec3 &position, const vec3 &center, const vec3 &up)
{
	rotation = GLToEulerRad(GLLookAt(position, center, up));
	this->position = position;
}

void Camera::setup(GLuint program, const mat4 &model)
{
	static mat4 view;
	static mat4 viewProjection;
	static mat3 normal;

	static mat4 modelViewProjection;

	view =					GLrotate(rotation) * GLTranslation(-position);
	viewProjection =		projection * view;
	normal =				mat3(transpose(inverse(model)));

	modelViewProjection =	viewProjection * model;

	/*if (program == 3)
	{
		glUniformMatrix4fv(transformLocations.model, 1, GL_TRUE, model.m);
		glUniformMatrix4fv(transformLocations.viewProjection, 1, GL_TRUE, viewProjection.m);
		glUniformMatrix3fv(transformLocations.normal, 1, GL_TRUE, normal.m);
		glUniform3fv(transformLocations.viewPosition, 1, position.v);
	}*/

	GLuint loc = glGetUniformLocation(program, "transform.modelViewProjection");
	glUniformMatrix4fv(loc, 1, GL_TRUE, modelViewProjection.m);
	loc = glGetUniformLocation(program, "transform.modelView");
	glUniformMatrix4fv(loc, 1, GL_TRUE, model.m);
}
