#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

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
	char ShowChar(); //ve que letra esta na posicao do cursor
	int BoardMainCoord(); //da a coordenada maxima do board na direcao atual
private:
	vector<vector<char>> board;
	int x, y;
	Cursor Cursor;
};

