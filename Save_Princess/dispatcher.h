#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include "camera.h"
#include "gl_vars.h"
#include "mesh.h"

using namespace std;

typedef unsigned int uint;

#define STRENGTH_PER_ATTACK 2
#define STRENGTH_REGEN 2
#define HP_REGEN 2

#define KNIGHT_MAX_STRENGTH 10
#define KNIGHT_MAX_HP 150
#define KNIGHT_ATTAK_POWER 3

#define PRINCESS_MAX_HP 1

#define ZOMBIE_MAX_HP 1
#define ZOMBIE_ATTAK_POWER 3

#define DRAGON_MAX_HP 20
#define DRAGON_ATTAK_POWER 5 

#define BORDER_W_PCNT 5

#define CELL_EMPTY '.'
#define CELL_KNIGHT 'K'
#define CELL_DRAGON 'D'
#define CELL_ZOMBIE 'Z'
#define CELL_WALL '#'
#define CELL_PRINCESS 'P'

class Character;
class Knight;
class Princess;
class Monster;
class Dragon;
class Zombie;

class GameDispatcher;

enum DIRECTION { DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT };

class Character
{
public:
	Character(GameDispatcher* dispatcher, uint x, uint y, uint hitPoints, GLuint tex);
	virtual void hit(const uint hit_value);
	virtual void render(GLuint program, Camera camera);
protected:
	virtual void die() = 0;
	virtual char get_symbol() = 0;

	GameDispatcher* _dispatcher;
	uint _x;
	uint _y;
	int _hitPoints;
	FlatMesh* _mesh;
	DIRECTION _look_to;
};

class Princess : public Character
{
public:
	Princess(GameDispatcher* dispatcher, uint x, uint y, GLuint tex) :
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
	MovableCharacter(GameDispatcher* dispatcher, uint x, uint y, uint hitPoints, GLuint tex) :
		Character(dispatcher, x, y, hitPoints, tex) {};
protected:
	void move(uint x, uint y);
};

class Knight : public MovableCharacter
{
public:
	Knight(GameDispatcher* dispatcher, uint x, uint y, GLuint tex) :
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
	void attack(uint x, uint y);
	uint _strength;
};

class Monster : public MovableCharacter
{
public:
	Monster(GameDispatcher* dispatcher, uint x, uint y, uint hitPoints, GLuint tex) :
		MovableCharacter(dispatcher, x, y, hitPoints, tex) { _isAlive = true; };
	void your_turn();
	pair<uint, uint> get_coords() { return {_x, _y}; };

	void render(GLuint program, Camera camera) override;
	virtual void hit(const uint hit_value) override;
protected:
	bool _isAlive;
	virtual void die() override;
	virtual void attack(uint x, uint y) = 0;
private:
	bool bfs_aux(
		pair<uint, uint> v, pair<uint, uint> to, set<pair<uint, uint>>& used, 
		map<pair<uint, uint>, pair<uint, uint>>& p, map<pair<uint, uint>, int>& d, 
		queue<pair<uint, uint>>& q, pair<uint, uint>& knight);
};

class Zombie : public Monster
{
public:
	Zombie(GameDispatcher* dispatcher, uint x, uint y, GLuint tex) :
		Monster(dispatcher, x, y, ZOMBIE_MAX_HP, tex) {};
	//virtual void your_turn()) override;
protected:
	virtual char get_symbol() override { return CELL_ZOMBIE; };
private:
	virtual void attack(uint x, uint y);
};

class Dragon : public Monster
{
public:
	Dragon(GameDispatcher* dispatcher, uint x, uint y, GLuint tex) :
		Monster(dispatcher, x, y, DRAGON_MAX_HP, tex) {};
	//virtual void your_turn()) override;
protected:
	virtual char get_symbol() override { return CELL_DRAGON; };
private:
	virtual void attack(uint x, uint y);
};

struct GameTextures
{
	GameTextures() : knightTex(-1), princessTex(-1), zombieTex(-1), dragonTex(-1),
		wallTex(-1), emptyTex(-1), borderTex(-1) {};
	GLuint knightTex, princessTex, zombieTex, dragonTex;
	GLuint wallTex, emptyTex, borderTex;
};

class GameDispatcher
{
public:
	GameDispatcher(GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY,
		const uint size, const  vector<string>& field);
	GameDispatcher(GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY,
		const uint size, const char** field);
	GameDispatcher(GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY,
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

	FlatMesh* _emptyMesh;
	FlatMesh* _border;
	vector<FlatMesh*> _walls;

	map<pair<uint, uint>, Character*> _characterMap;

	uint _fieldSize;
	vector<string> _field;
	void prepare_field();

	GameTextures _textures;

	uint _offsetX, _offsetY;
	uint _sizeInPixels;
};
/*
class GameInterface
{
public:

private:
	GameDispatcher _dispatcher;
};*/
