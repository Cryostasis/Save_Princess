#include "save_princess.h"
#include "gl_vars.h"
#include "camera.h"
#include "fonts.h"
#include "glerrors.h"
#include "textures.h"
#include "error_log.h"
#include "dispatcher.h"

#include <exception>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

GameTextures globalTextures;
GameInterface* mainInterface;

bool flagTactLocal = false;

void register_textures()
{
	globalTextures.borderTex		.resize(1);
	globalTextures.emptyTex			.resize(1);
	globalTextures.wallTex			.resize(1);
	globalTextures.knightTex		.resize(1);
	globalTextures.princessTex		.resize(1);
	globalTextures.dragonTex		.resize(1);
	globalTextures.zombieTex		.resize(1);

	/*globalTextures.borderTex 		[0].push_back(get_texture_from_tga("textures/border.tga"));
	globalTextures.emptyTex 		[0].push_back(get_texture_from_tga("textures/empty.tga"));
	globalTextures.wallTex 			[0].push_back(get_texture_from_tga("textures/wall.tga"));
	globalTextures.knightTex 		[0].push_back(get_texture_from_tga("textures/sych.tga"));
	globalTextures.princessTex 		[0].push_back(get_texture_from_tga("textures/princess.tga"));
	globalTextures.dragonTex 		[0].push_back(get_texture_from_tga("textures/putler.tga"));
	globalTextures.zombieTex 		[0].push_back(get_texture_from_tga("textures/liberator.tga"));*/

	globalTextures.borderTex 		[0].push_back(get_texture_from_tga("textures/empty.tga"));
	globalTextures.emptyTex 		[0].push_back(get_texture_from_tga("textures/grass.tga"));
	globalTextures.wallTex 			[0].push_back(get_texture_from_tga("textures/stone.tga"));
	globalTextures.knightTex 		[0].push_back(get_texture_from_tga("textures/steve.tga"));
	globalTextures.princessTex 		[0].push_back(get_texture_from_tga("textures/diamond.tga"));
	globalTextures.dragonTex 		[0].push_back(get_texture_from_tga("textures/dragon.tga"));
	globalTextures.zombieTex 		[0].push_back(get_texture_from_tga("textures/skeleton.tga"));
}

void render_all()
{
	if (!flagGameStarted)
		return;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WND_RES[0], WND_RES[1]);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	glUseProgram(textProgram);

	mainInterface->render(textProgram, textCamera);

	glutSwapBuffers();
	check_GL_error();
}

void init_scene()
{
	register_textures();

	glutWindow = glutGetWindow();

	mainInterface = new GameInterface(globalTextures);
	mainInterface->create_form();
	//mainDispatcher = new GameDispatcher(globalTextures, 600, 0, 0, testSize, testField);
}

void on_mouse_click(int button, int state, int x, int y)
{
	if (state)
		return;
	switch (button)
	{
	case GLUT_LEFT_BUTTON: {break; }
	}
}

void on_press_key(unsigned char key, int x, int y)
{
	switch (key)
	{

	}
}

void on_press_spec_key(int key, int x, int y)
{
	if (flagTactLocal || flagSleep)
		return;
	switch (key)
	{
	case GLUT_KEY_SHIFT_R: { mainInterface->skip(); break; }
	case GLUT_KEY_UP: { mainInterface->go_up(true); break; }
	case GLUT_KEY_DOWN: { mainInterface->go_down(true); break; }
	case GLUT_KEY_RIGHT: { mainInterface->go_right(true); break; }
	case GLUT_KEY_LEFT: { mainInterface->go_left(true); break; }
	}
}

void on_up_key(unsigned char key, int x, int y)
{
	switch (key)
	{

	}
}

void on_up_spec_key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L: {break; }
	}
}

void on_mouse_move(int x, int y)
{
	static bool mouse_flag;
	if (mouse_flag)
	{
		mouse_flag = false;
		return;
	}
}

void timer(int i)
{
	static long long last_ticks;

	if (flagTact)
	{
		last_ticks = GetTickCount();
		flagTact = false;
		flagTactLocal = true;
	}
	if (flagTactLocal && GetTickCount() - last_ticks > TIMER_DELAY)
		flagTactLocal = false;

	render_all();
	glutTimerFunc(TIMER_TICKS, timer, 0);
}

