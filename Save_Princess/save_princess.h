#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <exception>
#include <vector>
#include <map>

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

class GameDispatcher;

class Character
{
public:
	Character(uint x, uint y, uint hitPoints, GLuint tex)  {};
	virtual void your_turn(GameDispatcher& state) = 0;
	virtual void hit(const uint hit_value) = 0;
	void render();
protected:
	GLuint _texture;
	uint _x, _y;
	uint _hitPoints;
};

class Princess : public Character
{
public:
	Princess(uint x, uint y, GLuint tex) : Character(x, y, PRINCESS_MAX_HP, tex) {};
	virtual void hit(const uint hit_value) override;
	virtual void your_turn(GameDispatcher& state) override {};
};

class Knight : public Character
{
public:
	Knight(uint x, uint y, GLuint tex) : 
		Character(x, y, KNIGHT_MAX_HP, tex), _strength(KNIGHT_MAX_STRENGTH) {};
	virtual void your_turn(GameDispatcher& state) override;
	virtual void hit(const uint hit_value) override;
protected:
	void attack(uint x, uint y);
	uint _strength;
};

class Monster : public Character
{
public:
	Monster(uint x, uint y, uint hitPoints, GLuint tex) : Character(x, y, hitPoints, tex) {};
protected:
	virtual bool check_knight() = 0;
	virtual void attack(uint x, uint y) = 0;
};

class Zombie : public Monster
{
public:
	Zombie(uint x, uint y, GLuint tex) : Monster(x, y, ZOMBIE_MAX_HP, tex) {};
	virtual void your_turn(GameDispatcher& state) override;
	virtual void hit(const uint hit_value) override;
protected:
	virtual bool check_knight();
	virtual void attack(uint x, uint y);
};

class Dragon : public Monster
{
public:
	Dragon(uint x, uint y, GLuint tex) : Monster(x, y, DRAGON_MAX_HP, tex) {};
	virtual void your_turn(GameDispatcher& state) override;
	virtual void hit(const uint hit_value) override;
private:
	virtual bool check_knight();
	virtual void attack(uint x, uint y);
};

struct GameTextures
{
	GameTextures() : knightTex(-1), princessTex(-1), zombieTex(-1), dragonTex(-1),
		wallTex(-1), emptyTex(-1) {};
	GLuint knightTex, princessTex, zombieTex, dragonTex;
	GLuint wallTex, emptyTex;
};

class GameDispatcher
{
public:
	//GameState() : _field(0), _fieldSize(0) {};
	GameDispatcher(GameTextures textures, const uint size, const  std::vector<std::string>& field);
	GameDispatcher(GameTextures textures, const uint size, const char** field);
	GameDispatcher(GameTextures textures, const char* file);
	void render();
	void hit_xy(uint x, uint y) { }
private:
	std::vector<Monster*> _monsters;
	Knight* _knight;
	Princess* _princess;

	std::map<std::pair<uint, uint>, Character*> _characterMap;

	uint _fieldSize;
	std::vector<std::string> _field;
	void analyze_field();

	GameTextures _textures;

	uint _offsetX, _offsetY;
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