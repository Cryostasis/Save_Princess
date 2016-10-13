#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <vector>
#include <map>

#include "camera.h"
#include "gl_vars.h"
#include "mesh.h"
#include "control_panel.h"

void render();
void render_scene();
void on_mouse_click(int button, int state, int x, int y);
void on_press_key(unsigned char key, int x, int y);
void on_press_spec_key(int key, int x, int y);
void on_up_key(unsigned char key, int x, int y);
void on_up_spec_key(int key, int x, int y);
void on_mouse_move(int x, int y);
void timer(int i);
void render_all();
void init_scene();

extern GameInterface* mainInterface;
