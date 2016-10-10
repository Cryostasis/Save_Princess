#include "dispatcher.h"
#include "error_log.h"

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <exception>

using namespace std;

const set<char> USED_SYMBOLS = 
	{ CELL_DRAGON, CELL_EMPTY, CELL_KNIGHT, CELL_PRINCESS, CELL_WALL, CELL_ZOMBIE };
const set<char> MONSTER_SYMBOLS =
	{ CELL_DRAGON, CELL_ZOMBIE };

GameDispatcher::GameDispatcher(
	GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY,
	const uint size, const std::vector<std::string>& field) :
	_fieldSize(size), _field(field), _textures(textures), _sizeInPixels(pxSize),
	_offsetX(offsetX + BORDER_W_PCNT * pxSize / 100), _offsetY(offsetY + BORDER_W_PCNT * pxSize / 100)
{
	//GameDispatcher(textures, pxSize, offsetX, offsetY);
	prepare_field();
}

GameDispatcher::GameDispatcher(
	GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY, const uint size, const char ** field) :
	_fieldSize(size), _field(size), _textures(textures), _sizeInPixels(pxSize),
	_offsetX(offsetX + BORDER_W_PCNT * pxSize / 100), _offsetY(offsetY + BORDER_W_PCNT * pxSize / 100)
{
	//GameDispatcher(textures, pxSize, offsetX, offsetY);
	try
	{
		for (uint i = 0; i < size; i++)
			_field[i] = field[i];
	}
	catch (const std::exception& e)
	{
		cout << "�������� ������ ����";
		cerr << e.what();
	}

	prepare_field();
}

GameDispatcher::GameDispatcher(
	GameTextures textures, const uint pxSize, const uint offsetX, const uint offsetY, const char * file) :
	_textures(textures), _sizeInPixels(pxSize),
	_offsetX(offsetX + BORDER_W_PCNT * pxSize / 100), _offsetY(offsetY + BORDER_W_PCNT * pxSize / 100)
{
	//GameDispatcher(textures, pxSize, offsetX, offsetY);
	fstream fin;
	fin.open(file);

	fin >> _fieldSize;
	_field.resize(_fieldSize);
	for (uint i = 0; i < _fieldSize; i++)
		getline(fin, _field[i]);

	prepare_field();
}

void GameDispatcher::hit_xy(uint x, uint y, uint damage)
{
	for (int i = 0; i < _monsters.size(); i++)
		if (_monsters[i]->get_coords() == pair<uint, uint>(x, y))
			_monsters[i]->hit(damage);
}

void GameDispatcher::tact()
{
	for (int i = 0; i < _monsters.size(); i++)
		_monsters[i]->your_turn();
}

void GameDispatcher::prepare_field()
{
	int knightCnt = 0, princessCnt = 0;
	bool flagError = false;
	for (uint i = 0; i < _fieldSize; i++)
		for (uint j = 0; j < _fieldSize; j++)
		{
			if (USED_SYMBOLS.find(_field[i][j]) == USED_SYMBOLS.end())
				raise_error("Wrong field", false, true);
			switch (_field[i][j])
			{
			case CELL_KNIGHT:
			{
				if (++knightCnt > 1)
					throw std::exception("Too many knights");
				_knight = new Knight(this, j, i, _textures.knightTex);
				break;
			}
			case CELL_PRINCESS:
			{
				if (++princessCnt > 1)
					throw std::exception("Too many princesses");
				_princess = new Princess(this, j, i, _textures.princessTex);
				break;
			}
			case CELL_DRAGON: { _monsters.push_back(new Dragon(this, j, i, _textures.dragonTex)); break; }
			case CELL_ZOMBIE: { _monsters.push_back(new Zombie(this, j, i, _textures.zombieTex)); break; }
			case CELL_WALL:
			{
				_walls.push_back(new FlatMesh(WND_RES[0], WND_RES[1],
					j * get_cell_size() + _offsetX, i * get_cell_size() + _offsetY,
					_textures.wallTex, WND_ASPECT, get_cell_size()));
				break;
			}
			default:
				break;
			}
		}
	if (knightCnt != 1 || princessCnt != 1)
		throw std::exception("wrong field");

	_emptyMesh = new FlatMesh(WND_RES[0], WND_RES[1], _offsetX, _offsetY, _textures.emptyTex, WND_ASPECT, _sizeInPixels);
	_border = new FlatMesh(WND_RES[0], WND_RES[1],
		_offsetX - BORDER_W_PCNT * _sizeInPixels / 100, _offsetY - BORDER_W_PCNT * _sizeInPixels / 100,
		_textures.borderTex, WND_ASPECT, _sizeInPixels * (100 + BORDER_W_PCNT * 2) / 100);
}

void GameDispatcher::render(GLuint program, Camera camera)
{
	_border->render(program, camera);
	_emptyMesh->render(program, camera);
	for (int i = 0; i < _walls.size(); i++)
		_walls[i]->render(program, camera);

	for (int i = 0; i < _monsters.size(); i++)
		_monsters[i]->render(program, camera);
	_knight->render(program, camera);
	_princess->render(program, camera);
}

Character::Character(GameDispatcher* dispatcher, uint x, uint y, uint hitPoints, GLuint tex) :
	_x(x), _y(y), _hitPoints(hitPoints), _look_to(DIRECTION_UP), _dispatcher(dispatcher)
{
	_mesh = new FlatMesh(WND_RES[0], WND_RES[1],
		dispatcher->get_offset_X() + x * dispatcher->get_cell_size(),
		dispatcher->get_offset_Y() + y * dispatcher->get_cell_size(),
		tex, WND_ASPECT, dispatcher->get_cell_size());
};

void Character::render(GLuint program, Camera camera)
{
	_mesh->render(program, camera);
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

void Knight::hit(const uint hit_value)
{
}

void Knight::move_right(const bool attack)
{
	if (_x < 1 || _dispatcher->get_cell_chr(_x - 1, _y) == CELL_WALL)
		return;
	if (_dispatcher->get_cell_chr(_x - 1, _y) == CELL_EMPTY)
	{
		_dispatcher->set_cell_chr(_x, _y, CELL_EMPTY);
		_x = _x - 1;
		_dispatcher->set_cell_chr(_x, _y, CELL_KNIGHT);
		_mesh->move_to(
			_dispatcher->get_offset_X() + _x * _dispatcher->get_cell_size(),
			_dispatcher->get_offset_Y() + _y * _dispatcher->get_cell_size());
	}
	if (attack && _strength >= STRENGTH_PER_ATTACK)
	{
		_strength -= STRENGTH_PER_ATTACK;
		_dispatcher->hit_xy(_x - 1, _y, KNIGHT_ATTAK_POWER);
	}

	_strength += STRENGTH_REGEN;
	_hitPoints += HP_REGEN;

	_dispatcher->tact();
}

void Knight::move_left(const bool attack)
{
	
}

void Knight::move_down(const bool attack)
{

}

void Knight::move_up(const bool attack)
{
	if (_y < 1 || _dispatcher->get_cell_chr(_x, _y - 1) == CELL_WALL)
		return;
	if (_dispatcher->get_cell_chr(_x, _y - 1) == CELL_EMPTY)
	{
		_dispatcher->set_cell_chr(_x, _y, CELL_EMPTY);
		_y = _y - 1;
		_dispatcher->set_cell_chr(_x, _y, CELL_KNIGHT);
		_mesh->move_to(
			_dispatcher->get_offset_X() + _x * _dispatcher->get_cell_size(),
			_dispatcher->get_offset_Y() + _y * _dispatcher->get_cell_size());
	}
	if (attack && _strength >= STRENGTH_PER_ATTACK)
	{
		_strength -= STRENGTH_PER_ATTACK;
		_dispatcher->hit_xy(_x, _y - 1, KNIGHT_ATTAK_POWER);
	}

	_strength += STRENGTH_REGEN;
	_hitPoints += HP_REGEN;
}

void Knight::attack_forward()
{
}

void Knight::attack(uint x, uint y)
{

}

void Princess::hit(const uint hit_value)
{
}

void Monster::your_turn()
{
	bool flag = false;
	queue<pair<uint, uint>> q;
	q.push({_x, _y});
	uint sz = _dispatcher->get_size();

	set<pair<uint, uint>> used;
	pair<uint, uint> knight = { UINT_MAX , UINT_MAX };

	//vector<vector<bool>> used(sz);
	//for (int i = 0; i < sz; i++)
	//	used[i].resize(sz);
	map<pair<uint, uint>, int> d;
	map<pair<uint, uint>, pair<uint, uint>> p;
	used.insert({_x, _y});
	d[{_x, _y}] = 0;
	p[{_x, _y}] = { UINT_MAX, UINT_MAX };
	while (!q.empty() && !flag) 
	{
		pair<uint, uint> v = q.front();
		q.pop();
		if (v.first > 0 && 
			(_dispatcher->get_cell_chr(v.first - 1, v.second) == CELL_EMPTY
			|| _dispatcher->get_cell_chr(v.first - 1, v.second) == CELL_KNIGHT)
			&& used.find({ v.first - 1, v.second }) == used.end())
			flag = flag || bfs_aux(v, { v.first - 1, v.second }, used, p, d, q, knight);

		if (v.first < sz - 1 && 
			(_dispatcher->get_cell_chr(v.first + 1, v.second) == CELL_EMPTY
			|| _dispatcher->get_cell_chr(v.first + 1, v.second) == CELL_KNIGHT)
			&& used.find({ v.first + 1, v.second }) == used.end())
			flag = flag || bfs_aux(v, { v.first + 1, v.second }, used, p, d, q, knight);

		if (v.second > 0 && 
			(_dispatcher->get_cell_chr(v.first, v.second - 1) == CELL_EMPTY
			|| _dispatcher->get_cell_chr(v.first, v.second - 1) == CELL_KNIGHT)
			&& used.find({ v.first, v.second - 1 }) == used.end())
			flag = flag || bfs_aux(v, { v.first, v.second - 1 }, used, p, d, q, knight);

		if (v.second < sz - 1 && 
			(_dispatcher->get_cell_chr(v.first, v.second + 1) == CELL_EMPTY
			|| _dispatcher->get_cell_chr(v.first, v.second + 1) == CELL_KNIGHT)
			&& used.find({ v.first, v.second + 1 }) == used.end())
			flag = flag || bfs_aux(v, { v.first, v.second + 1 }, used, p, d, q, knight);
	}

	if (!flag)
		return;

	vector<pair<uint, uint>> path;
	pair<uint, uint> v = knight;
	
	while (true)
	{
		if (p[v] == pair<uint, uint>(_x, _y))
			break;
		v = p[v];
	}

	if (_dispatcher->get_cell_chr(v.first, v.second) == CELL_KNIGHT)
	{
		attack(v.first, v.second);
		return;
	}

	_dispatcher->set_cell_chr(_x, _y, CELL_EMPTY);
	_x = v.first;
	_y = v.second;
	_dispatcher->set_cell_chr(_x, _y, get_symbol());
	_mesh->move_to(
		_dispatcher->get_offset_X() + _x * _dispatcher->get_cell_size(),
		_dispatcher->get_offset_Y() + _y * _dispatcher->get_cell_size());

	if (abs((int)knight.first - (int)_x) + abs((int)knight.second - (int)_y) == 1)
		attack(knight.first, knight.second);
}

bool Monster::bfs_aux(
	pair<uint, uint> v, pair<uint, uint> to, set<pair<uint, uint>>& used, 
	map<pair<uint, uint>, pair<uint, uint>>& p, map<pair<uint, uint>, int>& d, 
	queue<pair<uint, uint>>& q, pair<uint, uint>& knight)
{
	used.insert(to);
	q.push(to);
	d[to] = d[v] + 1;
	p[to] = v;
	if (_dispatcher->get_cell_chr(to.first, to.second) == CELL_KNIGHT)
	{
		knight = to;
		return true;
	}
	return false;
}
