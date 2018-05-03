#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include "Cursor.h"

using namespace std;

class Board
{
public:
	Board(int x, int y);
	void display() const;
	void getWord();
	bool Verify(int posX, int posY, char direction, string palavra); //Ve se pode por a palavra
	void Insert(int posX, int posY, char direction, string palavra); //mete
	void ChangeChar(char letra); //altera no board a letra com base na posicao do cursor
	char ShowChar() const; //ve que letra esta na posicao do cursor
	int CoordLimit() const; //da a coordenada maxima do board na direcao atual (usa o cursor)
	vector<string> getPlacedWords() const;
private:
	vector<vector<char>> board;
	int x, y;
	map<string, string> placedWordsCoords; // "Palavra", "LCD"
	Cursor Cursor;
};
