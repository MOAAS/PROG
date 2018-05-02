#include "Board.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>
#include "consolecolors.h"

Board::Board(int x, int y) {
	vector<vector<char>> v(x, vector<char>(y, '.'));
	board = v;
	this->x = x; // + 1 valor
	this->y = y; // + 1 valor
}

void Board::display() const {
	const int N_ROWS = x;
	const int N_COLUMNS = y;
	char letras_cima = 'a'; // Limite superior terá letras minúsculas.
	char letras_lado = 'A'; // Limite inferior terá letras maiúsculas
	setcolor(12); // LIGHT RED
	cout << "   ";
	for (int i = 0; i < N_ROWS; i++)	{
		cout << letras_cima << ' ';
		letras_cima++;
	}
	cout << endl;
	for (int i = 0; i < N_COLUMNS; i++)	{
		cout << letras_lado << "  ";
		setcolor(0, 15); // BLACK, WHITE
		letras_lado++;
		for (int j = 0; j < N_ROWS; j++)
			cout << board[j][i] << ' ';
		setcolor(12); // LIGHT RED
		cout << endl;
	}
    setcolor(15); // WHITE
}

void Board::Principal()
{
	int  posicaoX, posicaoY;
	char direcao;
	//CUIDADOS COM INPUT 
	// '-' e '?'
	//
	string input;
	cin >> input;
	posicaoY = (int)input[0] - (int)'A';
	posicaoX = (int)input[1] - (int)'a';
	direcao = input[2];
	//loop ate input ser valido (coordenadas dentro dos limites ou ctrl-z) e coordenadas dentro dos limites!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	string palavra;
	cin >> palavra;
	//loop ate palavra ser valida (palavra ou + ou ?)
	//se = ? (sugerir) , se = - (apagar) 

	//verificar
	if (direcao == 'H')
		if (VerificaHorizontal(posicaoX, posicaoY, palavra))
			InsereHorizontal(posicaoX, posicaoY, palavra);
		else cout << "Invalido" << endl;
	else if (direcao == 'V')
		if (VerificaVertical(posicaoX, posicaoY, palavra))
			InsereVertical(posicaoX, posicaoY, palavra);
		else cout << "Invalido" << endl;
}
////////////////////////////
bool Board::VerificaHorizontal(int posicaoX, int posicaoY, string palavra)
{
	int tamanho = palavra.length();
	if (posicaoX + tamanho >= x)
		return false;
	if (posicaoX > 0) //verifica se antes tem letra
		if (!(board[posicaoX - 1][posicaoY] == '#' || board[posicaoX - 1][posicaoY] == '.'))
			return false;
	for (int i = 0; i<tamanho; i++)
	{
		if (!(board[posicaoX][posicaoY] == palavra[i] || board[posicaoX][posicaoY] == '.'))
			return false;
		posicaoX++;
	}
	if (posicaoX<x) //verifica se depois tem letra
		if (!(board[posicaoX][posicaoY] == '#' || board[posicaoX][posicaoY] == '.'))
			return false;
	return true;
}
void Board::InsereHorizontal(int posicaoX, int posicaoY, string palavra)
{
	int tamanho = palavra.length();
	//direcao horizontal;
	if (posicaoX > 0) //verifica se pode por # antes
		board[posicaoX - 1][posicaoY] = '#';
	for (int i = 0; i<tamanho; i++)
	{
		board[posicaoX][posicaoY] = palavra[i];
		posicaoX++;
	}
	if (posicaoX<x) //verifica se pode por # depois
		board[posicaoX][posicaoY] = '#';
}
//////////////////////////////////
bool Board::VerificaVertical(int posicaoX, int posicaoY, string palavra)
{
	int tamanho = palavra.length();
	if (posicaoX + tamanho >= y)
		return false;
	if (posicaoY > 0) //verifica se antes tem letra
		if (!(board[posicaoX][posicaoY - 1] == '#' || board[posicaoX][posicaoY - 1] == '.'))
			return false;
	for (int i = 0; i<tamanho; i++)
	{
		if (!(board[posicaoX][posicaoY] == palavra[i] || board[posicaoX][posicaoY] == '.'))
			return false;
		posicaoY++;
	}
	if (posicaoY<y) //verifica se depois tem letra
		if (!(board[posicaoX][posicaoY] == '#' || board[posicaoX][posicaoY] == '.'))
			return false;
	return true;
}
void Board::InsereVertical(int posicaoX, int posicaoY, string palavra)
{
	int tamanho = palavra.length();
	//direcao horizontal;
	if (posicaoY > 0) //verifica se pode por # antes
		board[posicaoX][posicaoY - 1] = '#';
	for (int i = 0; i<tamanho; i++)
	{
		board[posicaoX][posicaoY] = palavra[i];
		posicaoY++;
	}
	if (posicaoY<y) //verifica se pode por # depois
		board[posicaoX][posicaoY] = '#';
}
