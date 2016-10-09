#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <vector>

#include "camera.h"

typedef unsigned int uint;

#define KNIGHT_MAX_STRENGTH 10
#define KNIGHT_MAX_HP 10
#define KNIGHT_ATTAK_POWER 3

#define PRINCESS_MAX_HP 1

#define ZOMBIE_MAX_HP 1
#define ZOMBIE_ATTAK_POWER 3

#define DRAGON_MAX_HP 20
#define DRAGON_ATTAK_POWER 5 

class Character;
class Knight;
class Princess;
class Monster;
class Dragon;
class Zombie;

class GameState;

class FieldPoint
{
public:
	FieldPoint(uint x, uint y) : _x(x), _y(y) {};
	virtual bool is_something() = 0;
	virtual void hit(const uint hit_value) = 0;
	virtual void your_turn(GameState& state) {};
	virtual void render() = 0;
protected:
	uint _x, _y;
};

class Nothing : public FieldPoint
{
public:
	Nothing(uint x, uint y) : FieldPoint(x, y) {};
	virtual bool is_something() { return false; };
	virtual void render();
};

class Something : public FieldPoint
{
public:
	Something(uint x, uint y) : FieldPoint(x, y) {};
	virtual bool is_something() { return true; };
};

class Wall : public Something
{
public:
	Wall(uint x, uint y) : Something(x, y) {};
	virtual void render();
};

class Character : public Something
{
public:
	Character(uint x, uint y, uint hitPoints) : Something(x, y) {};
protected:
	uint _hitPoints;
};

class Princess : public Character
{
public:
	Princess(uint x, uint y) : Character(x, y, PRINCESS_MAX_HP) {};
	virtual void render();
	virtual void hit(const uint hit_value);
};

class Knight : public Character
{
public:
	Knight(uint x, uint y) : Character(x, y, KNIGHT_MAX_HP) {};
	virtual void your_turn(GameState& state);
protected:
	void attack();
	uint _strength;
};

class Monster : public Character
{
public:
	Monster(uint x, uint y, uint hitPoints) : Character(x, y, 0) {};
	virtual void your_turn(GameState& state);
protected:
	virtual bool check_knight() = 0;
	virtual void attack() = 0;
};

class Zombie : public Monster
{
public:
	Zombie(uint x, uint y) : Monster(x, y, ZOMBIE_MAX_HP) {};
protected:
	virtual bool check_knight();
	virtual void attack();
};

class Dragon : public Monster
{
public:
	Dragon(uint x, uint y) : Monster(x, y, DRAGON_MAX_HP) {};
private:
	virtual bool check_knight();
	virtual void attack();
};

class GameState
{
public:
	GameState() : _textField(0), _fieldSize(0) {};
	GameState(const uint size, const  std::vector<std::string>& field);
	GameState(const uint size, const char** field);
	GameState(const char* file);
private:
	std::vector<std::vector<FieldPoint*>> _field;
	uint _fieldSize;
	std::vector<std::string> _textField;
	void analize_field();
};

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