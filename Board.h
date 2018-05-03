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
	void InsereHorizontal(int posicaoX, int posicaoY, string palavra);
	void InsereVertical(int posicaoX, int posicaoY, string palavra);
	bool VerificaHorizontal(int posicaoX, int posicaoY, string palavra);
	bool VerificaVertical(int posicaoX, int posicaoY, string palavra);

	bool VerificaCursor(int posicaoX, int posicaoY, char direcao, string palavra); //Ve se pode por a palavra
	void InsereCursor(int posicaoX, int posicaoY, char direcao, string palavra); //mete
	void MudaCursor(char letra); //altera no board a letra com base na posicao do cursor
	char VeCursor(); //ve que letra esta na posicao do cursor
	int boardmaincoord(); //da a coordenada maxima do board na direcao correspondente
private:
	vector<vector<char>> board;
	int x, y;
	Cursor Cursor;
};
