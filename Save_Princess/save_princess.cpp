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

const vector<string> testField =
{
	"P...#",
	"#.#..",
	"D.#..",
	"Z...#",
	"....K"
};
const uint testSize = testField.size();

GameDispatcher* mainDispatcher;

vector<TextMesh*> texts(0);
FlatMesh* sych;
GameTextures globalTextures;

void register_textures()
{
	globalTextures.borderTex		.resize(1);
	globalTextures.emptyTex			.resize(1);
	globalTextures.wallTex			.resize(1);
	globalTextures.knightTex		.resize(1);
	globalTextures.princessTex		.resize(1);
	globalTextures.dragonTex		.resize(1);
	globalTextures.zombieTex		.resize(1);

	globalTextures.borderTex 		[0].push_back(get_texture_from_tga("textures/border.tga"));
	globalTextures.emptyTex 		[0].push_back(get_texture_from_tga("textures/empty.tga"));
	globalTextures.wallTex 			[0].push_back(get_texture_from_tga("textures/wall.tga"));
	globalTextures.knightTex 		[0].push_back(get_texture_from_tga("textures/sych.tga"));
	globalTextures.princessTex 		[0].push_back(get_texture_from_tga("textures/princess.tga"));
	globalTextures.dragonTex 		[0].push_back(get_texture_from_tga("textures/putler.tga"));
	globalTextures.zombieTex 		[0].push_back(get_texture_from_tga("textures/liberator.tga"));
}

void render()
{
	SetFocus(GetConsoleWindow());
	glUseProgram(textProgram);

	mainDispatcher->render(textProgram, textCamera);

	texts[0]->render(textProgram, textCamera);
	//sych->render(textProgram, textCamera);

	glutSwapBuffers();
	check_GL_error();
}

void render_scene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WND_RES[0], WND_RES[1]);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	render();
}

void init_scene()
{
	register_textures();

	mainDispatcher = new GameDispatcher(globalTextures, 600, 0, 0, testSize, testField);

	
	texts.resize(1);
	texts[0] = new TextMesh(WND_RES[0], WND_RES[1], 500, 500, "text", vec4(1.0, 0.0, 1.0, 1.0), WND_ASPECT, 40);
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
	switch (key)
	{
	case GLUT_KEY_UP: { mainDispatcher->go_up(false); break; }
	case GLUT_KEY_DOWN: { mainDispatcher->go_down(false); break; }
	case GLUT_KEY_RIGHT: { mainDispatcher->go_right(false); break; }
	case GLUT_KEY_LEFT: { mainDispatcher->go_left(false); break; }

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
	static bool init;
	static long long last_ticks;
	if (!init)
		last_ticks = GetTickCount();
	init = true;

	last_ticks = GetTickCount();

	render_all();
	glutTimerFunc(TIMER_TICKS, timer, 0);
}


void render_all()
{
	render_scene();
}


