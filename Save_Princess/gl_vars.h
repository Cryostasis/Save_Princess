#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera.h"

const int WND_RESOLUTION[2] = { 1280, 720 };
const int WND_POSITION[2] = { 70, 0 };
const int TIMER_TICKS = 1;
const GLfloat WND_ASPECT = (float)WND_RESOLUTION[0] / (float)WND_RESOLUTION[1];

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
