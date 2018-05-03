#include "stdafx.h"
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

void Board::getWord()
{
	int posX, posY;
	char direction;
	//CUIDADOS COM INPUT 
	// '-' e '?'
	//
	string input; cin >> input;
	posY = (int)input[0] - (int)'A';
	posX = (int)input[1] - (int)'a';
	direction = input[2];
	//loop ate input ser valido (coordenadas dentro dos limites ou ctrl-z) e coordenadas dentro dos limites!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	string palavra;
	cin >> palavra;
	//loop ate palavra ser valida (palavra ou + ou ?)
	//se = ? (sugerir) , se = - (apagar) 

	//verificar
	if (direction == 'H')
		if (VerificaHorizontal(posX, posY, palavra))
			InsereHorizontal(posX, posY, palavra);
		else cout << "Invalido" << endl;
	else if (direction == 'V')
		if (VerificaVertical(posX, posY, palavra))
			InsereVertical(posX, posY, palavra);
		else cout << "Invalido" << endl;
}
////////////////////////////
bool Board::VerificaHorizontal(int posX, int posY, string palavra)
{
	int tamanho = palavra.length();
	if (posX + tamanho >= x) // Se nao couber.
		return false;
	if (posX > 0 && board[posX - 1][posY] != '#' && board[posX - 1][posY] != '.') // Se tiver espaço à esquerda, e se o espaço nao for nem '#' nem '.', retorna false.
		return false;
	for (int i = 0; i<tamanho; i++) // Verifica cada espaço um a um.
	{
		if (board[posX][posY] != palavra[i] && board[posX][posY] != '.') // O espaço tem de ser igual à letra que se quer colocar, ou estar vazio.
			return false;
		posX++;
	}
	if (posX < x && board[posX][posY] != '#' && board[posX][posY] != '.') // Se tiver espaço à esquerda, e se o espaço nao for nem '#' nem '.', retorna false.
		return false;
	return true;
}

void Board::InsereHorizontal(int posX, int posY, string palavra)
{
	int tamanho = palavra.length();
	//direcao horizontal;
	if (posX > 0) //verifica se pode por # antes
		board[posX - 1][posY] = '#';
	for (int i = 0; i<tamanho; i++)
	{
		board[posX][posY] = palavra[i];
		posX++;
	}
	if (posX<x) //verifica se pode por # depois
		board[posX][posY] = '#';
}
//////////////////////////////////
bool Board::VerificaVertical(int posX, int posY, string palavra)
{
	int tamanho = palavra.length();
	if (posX + tamanho >= y) // Se nao couber
		return false;
	if (posY > 0 && board[posX][posY - 1] != '#' && board[posX][posY - 1] != '.')
			return false;
	for (int i = 0; i<tamanho; i++)
	{
		if (board[posX][posY] != palavra[i] && board[posX][posY] != '.')
			return false;
		posY++;
	}
	if (posY < y && board[posX][posY] != '#' && board[posX][posY] != '.')
			return false;
	return true;
}
void Board::InsereVertical(int posX, int posY, string palavra)
{
	int tamanho = palavra.length();
	//direcao vertical;
	if (posY > 0) //verifica se pode por # antes
		board[posX][posY - 1] = '#';
	for (int i = 0; i<tamanho; i++)
	{
		board[posX][posY] = palavra[i];
		posY++;
	}
	if (posY<y) //verifica se pode por # depois
		board[posX][posY] = '#';
}

///////////////////////////////////////////////////CURSOR///////////////////////////////////////////////////////////
bool Board::VerificaCursor(int posicaoX, int posicaoY,char direcao, string palavra)
{
	Cursor.moveTo(posicaoX, posicaoY, direcao);
	int tamanho = palavra.length();
	if (Cursor.maincoord() + tamanho >= boardmaincoord())
		return false;
	if (Cursor.maincoord() > 0) //verifica se antes tem letra
	{
		Cursor--;
		if (!(VeCursor() == '#' || VeCursor() == '.'))
			return false;
		Cursor++;
	}
	for (int i = 0; i < tamanho; i++)
	{
		if (!(VeCursor() == palavra[i] || VeCursor() == '.'))
			return false;
		Cursor++;
	}
	if (Cursor.maincoord() < boardmaincoord()) //verifica se depois tem letra
		if (!(VeCursor() == '#' || VeCursor() == '.'))
			return false;
	return true;
}

void Board::InsereCursor(int posicaoX, int posicaoY, char direcao, string palavra)
{
	Cursor.moveTo(posicaoX, posicaoY, direcao);
	int tamanho = palavra.length();
	if (Cursor.maincoord() > 0)
	{
		Cursor--;
		MudaCursor('#');
		Cursor++;
	}
	for (int i = 0; i<tamanho; i++)
	{
		MudaCursor(palavra[i]);
		Cursor++;
	}
	if (Cursor.maincoord()<boardmaincoord()) //verifica se pode por # depois
		MudaCursor('#');
}

void Board::MudaCursor(char letra)
{
	board[Cursor.x][Cursor.y] = letra;
}
char Board::VeCursor()
{
	return board[Cursor.x][Cursor.y];
}

int Board::boardmaincoord()
{
	if (Cursor.dir == 'H')
		return x;
	else return y;
}
