#pragma once
#include <string>

using namespace std;

class Cursor {
public:
	friend class Board;
	Cursor(); // Default: (0, 0) Dir: H
	Cursor(int x, int y, char dir); // (x, y) Dir: dir
	void moveTo(int x, int y); // Altera (x, y)
	void moveTo(int x, int y, char dir); // Altera direção também.
	void moveTo(string coords); // Consegue receber coordenadas da forma "LcD"
	void changeDir(char dir); // Altera só direção.
	Cursor operator++(int unused); // Avança uma posição, dependendo da coordenada (Cursor++)
	Cursor operator--(int unused); // Diminui uma posição, dependendo da coordenada (Cursor--)
	int MainCoord(); // Devolve a coordenada na direção atual (x, caso dir = 'H'. y, caso dir = 'V')
private:
	int x, y;
	char dir; // Direção. H / V
};

int LCDtoPosX(string LCD); // recebe string da forma "LinhaColunaDireçao" devolve a coordenada X correspondente.
int LCDtoPosY(string LCD); // recebe string da forma "LinhaColunaDireçao" devolve a coordenada Y correspondente.
char LCDtoDir(string LCD); // recebe string da forma "LinhaColunaDireçao" devolve a direçao correspondente.
