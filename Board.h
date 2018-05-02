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
	void Mostrador() const;
	void Principal();	
	void InsereHorizontal(int posicaoX, int posicaoY, string palavra);
	void InsereVertical(int posicaoX, int posicaoY, string palavra);
	bool VerificaHorizontal(int posicaoX, int posicaoY, string palavra);
	bool VerificaVertical(int posicaoX, int posicaoY, string palavra);

private:
	vector<vector<char>> board;
	int x, y;
};
