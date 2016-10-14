#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <sstream>

//#include "control_panel.h"
#include "camera.h"
#include "gl_vars.h"
#include "mesh.h"
#include "fonts.h"

using namespace std;

typedef unsigned int uint;

#define CELL_EMPTY '.'
#define CELL_KNIGHT 'K'
#define CELL_DRAGON 'D'
#define CELL_ZOMBIE 'Z'
#define CELL_WALL '#'
#define CELL_PRINCESS 'P'

extern bool flagGameStarted;

const set<char> USED_SYMBOLS =
{ CELL_DRAGON, CELL_EMPTY, CELL_KNIGHT, CELL_PRINCESS, CELL_WALL, CELL_ZOMBIE };
const set<char> MONSTER_SYMBOLS =
{ CELL_DRAGON, CELL_ZOMBIE };
/*
#define STRENGTH_PER_ATTACK 2
#define STRENGTH_REGEN 2
#define HP_REGEN 2

#define KNIGHT_MAX_STRENGTH 10
#define KNIGHT_MAX_HP 12
#define KNIGHT_ATTAK_POWER 3

#define PRINCESS_MAX_HP 1

#define ZOMBIE_MAX_HP 1
#define ZOMBIE_ATTAK_POWER 3

#define DRAGON_MAX_HP 20
#define DRAGON_ATTAK_POWER 5 
*/

extern uint STRENGTH_PER_ATTACK;
extern uint STRENGTH_REGEN;
extern uint HP_REGEN;

extern uint KNIGHT_MAX_STRENGTH;
extern uint KNIGHT_MAX_HP;
extern uint KNIGHT_ATTAK_POWER;

extern uint PRINCESS_MAX_HP;

extern uint ZOMBIE_MAX_HP;
extern uint ZOMBIE_ATTAK_POWER;

extern uint DRAGON_MAX_HP;
extern uint DRAGON_ATTAK_POWER;

#define BORDER_W_PCNT 5
#define BORDER_W_PX 20

#define HP_TEXT_PRECENT 20
const vec4 HP_TEXT_COLOR = {1.0, 0.0, 0.0, 1.0};

class Character;
class Knight;
class Princess;
class Monster;
class Dragon;
class Zombie;

class GameDispatcher;

enum DIRECTION { DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT };

using textures_ptr = vector<vector<GLuint>>*;

class AnimatedMesh
{
public:
	virtual void render(const GLuint program, Camera camera) { _mesh.render(program, camera); };
	AnimatedMesh(const uint x, const uint y, textures_ptr textures, const float scale) :
		_mesh(WND_RES[0], WND_RES[1], x, y, (*textures)[0][0], WND_ASPECT, scale), _textures(textures) {};
protected:
	FlatMesh _mesh;
	uint _cellSize;
	textures_ptr _textures;
};

class CellMesh : public AnimatedMesh
{
public:
	CellMesh(const uint offsetX, const uint offsetY, const uint x, const uint y, const uint cellSize, 
		textures_ptr textures) : 
		AnimatedMesh(x * cellSize + offsetX, y * cellSize + offsetY, textures, cellSize), _cellSize(cellSize),
		_offsetX(offsetX), _offsetY(offsetY), _x(x), _y(y) {};
	virtual void move_to(const uint x, const uint y) { _mesh.move_to(x * _cellSize + _offsetX, y * _cellSize + _offsetY); }
protected:
	uint _cellSize;
	uint _offsetX, _offsetY;
	uint _x, _y;
};

class CellMeshHP : public CellMesh
{
public:
	virtual void render(const GLuint program, Camera camera) override;
	CellMeshHP(const uint offsetX, const uint offsetY, const uint x, const uint y, const uint cellSize, uint hp,
		textures_ptr textures);
	virtual void move_to(const uint x, const uint y) override;
	void set_HP(uint hp);
private:
	string _hpStr;
	uint _hp;
	TextMesh _text;
};

class Character
{
public:
	Character(GameDispatcher* dispatcher, const uint x, const uint y, const uint hitPoints, textures_ptr tex);
	virtual void hit(const uint hit_value);
	virtual void render(const GLuint program, Camera camera);
protected:
	virtual void die() = 0;
	virtual char get_symbol() = 0;

	GameDispatcher* _dispatcher;
	uint _x;
	uint _y;
	int _hitPoints;
	CellMeshHP _mesh;
	DIRECTION _look_to;
};

class Princess : public Character
{
public:
	Princess(GameDispatcher* dispatcher, const uint x, const uint y, textures_ptr tex) :
		Character(dispatcher, x, y, PRINCESS_MAX_HP, tex) {};
	virtual void hit(const uint hit_value) override;
	void check_knight();
protected:
	virtual void die() override;
	virtual char get_symbol() override { return CELL_PRINCESS; };
};

class MovableCharacter : public Character
{
public:
	MovableCharacter(GameDispatcher* dispatcher, const uint x, const uint y, const uint hitPoints, textures_ptr tex) :
		Character(dispatcher, x, y, hitPoints, tex) {};
protected:
	void move(const uint x, const uint y);
};

class Knight : public MovableCharacter
{
public:
	Knight(GameDispatcher* dispatcher, const uint x, const uint y, textures_ptr tex) :
		MovableCharacter(dispatcher, x, y, KNIGHT_MAX_HP, tex), _strength(KNIGHT_MAX_STRENGTH) {};
	virtual void hit(const uint hit_value) override;
	void move_up(const bool attack);
	void move_down(const bool attack);
	void move_left(const bool attack);
	void move_right(const bool attack);
	void attack_forward();
protected:
	virtual void die() override;
	virtual char get_symbol() override { return CELL_KNIGHT; };
private:
	void tact();
	void move_aux(const bool attack, const uint x, const uint y);
	void attack(const uint x, const uint y);
	uint _strength;
};

class Monster : public MovableCharacter
{
public:
	Monster(GameDispatcher* dispatcher, const uint x, const uint y, const uint hitPoints, textures_ptr tex) :
		MovableCharacter(dispatcher, x, y, hitPoints, tex) { _isAlive = true; };
	void your_turn();
	pair<uint, uint> get_coords() { return {_x, _y}; };

	void render(const GLuint program, Camera camera) override;
	virtual void hit(const uint hit_value) override;
protected:
	bool _isAlive;
	virtual void die() override;
	virtual void attack(const uint x, const uint y) = 0;
private:
	bool bfs_aux_step(
		pair<uint, uint> v, pair<uint, uint> to, set<pair<uint, uint>>& used, 
		map<pair<uint, uint>, pair<uint, uint>>& p, map<pair<uint, uint>, int>& d, 
		queue<pair<uint, uint>>& q, pair<uint, uint>& knight);
	bool bfs_aux_check(
		pair<uint, uint> v, pair<uint, uint> to, set<pair<uint, uint>>& used,
		map<pair<uint, uint>, pair<uint, uint>>& p, map<pair<uint, uint>, int>& d,
		queue<pair<uint, uint>>& q, pair<uint, uint>& knight);
};

class Zombie : public Monster
{
public:
	Zombie(GameDispatcher* dispatcher, const uint x, const uint y, textures_ptr tex) :
		Monster(dispatcher, x, y, ZOMBIE_MAX_HP, tex) {};
protected:
	virtual char get_symbol() override { return CELL_ZOMBIE; };
private:
	virtual void attack(const uint x, const uint y);
};

class Dragon : public Monster
{
public:
	Dragon(GameDispatcher* dispatcher, const uint x, const uint y, textures_ptr tex) :
		Monster(dispatcher, x, y, DRAGON_MAX_HP, tex) {};
	//virtual void your_turn()) override;
protected:
	virtual char get_symbol() override { return CELL_DRAGON; };
private:
	virtual void attack(const uint x, const uint y);
};

struct GameTextures
{
	GameTextures() : knightTex(0), princessTex(0), zombieTex(0), dragonTex(0),
		wallTex(0), emptyTex(0), borderTex(0) {};
	vector<vector<GLuint>> knightTex, princessTex, zombieTex, dragonTex;
	vector<vector<GLuint>> wallTex, emptyTex, borderTex;
};

class GameDispatcher
{
public:
	GameDispatcher(GameTextures& textures, const uint pxSize, const uint offsetX, const uint offsetY,
		const uint size, const  vector<string>& field);
	GameDispatcher(GameTextures& textures, const uint pxSize, const uint offsetX, const uint offsetY,
		const uint size, const char** field);
	GameDispatcher(GameTextures& textures, const uint pxSize, const uint offsetX, const uint offsetY,
		const char* file);

	void render(GLuint program, Camera camera);
	
	void go_up(const bool attack) { _knight->move_up(attack); tact(); };
	void go_down(const bool attack) { _knight->move_down(attack); tact(); };
	void go_right(const bool attack) { _knight->move_right(attack); tact(); };
	void go_left(const bool attack) { _knight->move_left(attack); tact(); };
	void attack_forward() { _knight->attack_forward(); };

	uint get_offset_X() { return _offsetX; };
	uint get_offset_Y() { return _offsetY; };
	uint get_size() { return _fieldSize; };
	uint get_size_in_pixels() { return _sizeInPixels; };
	uint get_cell_size() { return _sizeInPixels / _fieldSize; };

	char get_cell_chr(uint x, uint y) { return _field[y][x]; };
	void set_cell_chr(uint x, uint y, char symbol) { _field[y][x] = symbol; };
	void hit_xy(uint x, uint y, uint damage);
	void hit_knight(uint damage);

	void tact();
	void game_over();
	void success();
private:
	vector<Monster*> _monsters;
	Knight* _knight;
	Princess* _princess;

	vector<vector<AnimatedMesh*>> _emptyMesh;
	AnimatedMesh* _border;
	vector<AnimatedMesh*> _walls;

	map<pair<uint, uint>, Character*> _characterMap;

	uint _fieldSize;
	vector<string> _field;
	void prepare_field();

	GameTextures& _textures;

	uint _offsetX, _offsetY;
	uint _sizeInPixels;
};
