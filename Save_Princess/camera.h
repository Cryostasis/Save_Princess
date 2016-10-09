#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math/math3d.h"
#include "math/mathgl.h"

class Camera
{
public:
	Camera();
	Camera(float x, float y, float z);
	void calc_perspective(float fov, float aspect, float zNear, float zFar);
	void calc_orto(float left, float right, float bottom, float top, float zNear, float zFar);
	void rotate(float x, float y, float z);
	void move(float x, float y, float z);
	void move_to(float x, float y, float z);
	void move_to(vec3 vec);
	void Camera::look_at(const vec3 &position, const vec3 &center, const vec3 &up);
	void setup(GLuint program, const mat4 &model);
private:
	vec3 position;
	vec3 rotation;
	mat4 projection;
};


