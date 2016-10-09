#include "save_princess.h"
#include "gl_vars.h"
#include "camera.h"
#include "fonts.h"
#include "glerrors.h"
#include "textures.h"

#include <exception>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

vector<TextMesh*> texts(0);
FlatMesh* sych;


void render()
{
	glUseProgram(textProgram);

	texts[0]->render(textProgram, textCamera);
	sych->render(textProgram, textCamera);

	glutSwapBuffers();
	check_GL_error();
}

void render_scene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WND_RESOLUTION[0], WND_RESOLUTION[1]);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	render();
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
	case GLUT_KEY_SHIFT_L: { break; }

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

void init_scene()
{
	GLuint tex = get_texture_from_tga("textures/sych.tga");
	sych = new FlatMesh(WND_RESOLUTION[0], WND_RESOLUTION[0], 0, 0, tex, WND_ASPECT, 72);

	texts.resize(1);
	texts[0] = new TextMesh(
		WND_RESOLUTION[0], WND_RESOLUTION[1], 1000, 200, "text", vec4(1.0, 1.0, 1.0, 1.0), WND_ASPECT, 40);
}

GameState::GameState(const uint size, const std::vector<std::string>& field)
{
	_fieldSize = size;
	_textField = field;

	analize_field();
}

GameState::GameState(const uint size, const char ** field)
{
	_fieldSize = size;
	_textField.resize(size);
	try
	{
		for (uint i = 0; i < size; i++)
			_textField[i] = field[i];
	}
	catch (const std::exception& e)
	{
		cout << "Неверный размер поля";
		cerr << e.what();
	}

	analize_field();
}

GameState::GameState(const char * file)
{
	fstream fin;
	fin.open(file);

	fin >> _fieldSize;
	_textField.resize(_fieldSize);
	for (uint i = 0; i < _fieldSize; i++)
		getline(fin, _textField[i]);

	analize_field();
}

void GameState::analize_field()
{

}

void Nothing::render()
{
}

void Wall::render()
{
}
