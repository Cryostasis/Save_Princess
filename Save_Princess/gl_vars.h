#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera.h"

const int WND_RES[2] = { 700, 700 };
const int WND_POS[2] = { 70, 0 };
const int TIMER_TICKS = 1;
const GLfloat WND_ASPECT = (float)WND_RES[0] / (float)WND_RES[1];

extern GLuint mainProgram;
extern GLuint textProgram;

//extern GLuint 

struct TransformLocations
{
	GLint model;
	GLint viewProjection;
	GLint normal;
	GLint viewPosition;
};

extern TransformLocations transformLocations;

extern Camera mainCamera;
extern Camera textCamera;

extern GLint glutWindow;
