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
#include <set>

using namespace std;

const set<char> usedSymbols = {'.', '#', 'K', 'D', 'P', 'Z'};

vector<TextMesh*> texts(0);
FlatMesh* sych;
GameTextures globalTextures;

void register_textures()
{
	globalTextures.emptyTex =		get_texture_from_tga("textures/empty.tga");
	globalTextures.wallTex =		get_texture_from_tga("textures/wall.tga");
	globalTextures.knightTex =		get_texture_from_tga("textures/sych.tga");
	globalTextures.princessTex =	get_texture_from_tga("textures/princess.tga");
	globalTextures.dragonTex =		get_texture_from_tga("textures/putler.tga");
	globalTextures.zombieTex =		get_texture_from_tga("textures/liberator.tga");
}

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
	register_textures();

	sych = new FlatMesh(
		WND_RESOLUTION[0], WND_RESOLUTION[0], 100, 100, 
		globalTextures.princessTex, WND_ASPECT, 400);

	texts.resize(1);
	texts[0] = new TextMesh(
		WND_RESOLUTION[0], WND_RESOLUTION[1], 100, 100, 
		"text", vec4(1.0, 1.0, 1.0, 1.0), WND_ASPECT, 40);

	sych->move_to(300, 300);
}

GameDispatcher::GameDispatcher(
	GameTextures textures, const uint size, const std::vector<std::string>& field) :
	_fieldSize(size), _field(field), _textures(textures)
{
	analyze_field();
}

GameDispatcher::GameDispatcher(GameTextures textures, const uint size, const char ** field) :
	_fieldSize(size), _field(size), _textures(textures)
{
	try
	{
		for (uint i = 0; i < size; i++)
			_field[i] = field[i];
	}
	catch (const std::exception& e)
	{
		cout << "Неверный размер поля";
		cerr << e.what();
	}

	analyze_field();
}

GameDispatcher::GameDispatcher(GameTextures textures, const char * file) : _textures(textures)
{
	fstream fin;
	fin.open(file);

	fin >> _fieldSize;
	_field.resize(_fieldSize);
	for (uint i = 0; i < _fieldSize; i++)
		getline(fin, _field[i]);

	analyze_field();
}

void GameDispatcher::render()
{

}

void GameDispatcher::analyze_field()
{
	int knightCnt = 0, princessCnt = 0;
	bool flagError = false;
	for (uint i = 0; i < _fieldSize; i++)
		for (uint j = 0; j < _fieldSize; j++)
		{
			if (usedSymbols.find(_field[i][j]) == usedSymbols.end())
				throw std::exception("Wrong field");
			switch (_field[i][j])
			{
			case 'K':
			{
				if (++knightCnt > 1)
					throw std::exception("Too many knights");
				_knight = new Knight(i, j, _textures.knightTex);
				break;
			}
			case 'P':
			{
				if (++princessCnt > 1)
					throw std::exception("Too many princesses");
				_princess = new Princess(i, j, _textures.princessTex);
				break;
			}
			case 'D':
			{
				_monsters.resize(_monsters.size() + 1);
				_monsters[_monsters.size() - 1] = new Dragon(i, j, _textures.princessTex);
				break;
			}
			case 'Z':
			{
				_monsters.resize(_monsters.size() + 1);
				_monsters[_monsters.size() - 1] = new Zombie(i, j, _textures.princessTex);
				break;
			}
			default:
				break;
			}
		}
	if (knightCnt != 1 || princessCnt != 1)
		throw std::exception("wrong field");
}

void Character::render()
{

}

void Dragon::your_turn(GameDispatcher & state)
{
}

void Dragon::hit(const uint hit_value)
{
}

bool Dragon::check_knight()
{
	return false;
}

void Dragon::attack(uint x, uint y)
{
}

void Zombie::your_turn(GameDispatcher & state)
{
}

void Zombie::hit(const uint hit_value)
{
}

bool Zombie::check_knight()
{
	return false;
}

void Zombie::attack(uint x, uint y)
{
}

void Knight::your_turn(GameDispatcher & state)
{
}

void Knight::hit(const uint hit_value)
{
}

void Knight::attack(uint x, uint y)
{
}

void Princess::hit(const uint hit_value)
{
}
