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
	this->x = x;
	this->y = y; 
}

void Board::display() const {
	const int N_ROWS = x;
	const int N_COLUMNS = y;
	char letras_cima = 'a'; // Limite superior terá letras minúsculas.
	char letras_lado = 'A'; // Limite inferior terá letras maiúsculas
	setcolor(LIGHTRED); // LIGHT RED
	cout << "   ";
	for (int i = 0; i < N_ROWS; i++) {
		cout << letras_cima << ' ';
		letras_cima++;
	}
	cout << endl;
	for (int i = 0; i < N_COLUMNS; i++) {
		cout << letras_lado << "  ";
		setcolor(BLACK,WHITE); // BLACK, WHITE
		letras_lado++;
		for (int j = 0; j < N_ROWS; j++)
			cout << board[j][i] << ' ';
		setcolor(LIGHTRED); // LIGHT RED
		cout << endl;
	}
	setcolor(WHITE); // WHITE
}

void Board::getWord()
{
	int posX, posY;
	char direction;
	//CUIDADOS COM INPUT 
	// '-' e '?'
	string input;
	cin >> input;
	posY = (int)input[0] - (int)'A';
	posX = (int)input[1] - (int)'a';
	direction = input[2];
	//loop ate input ser valido (coordenadas dentro dos limites ou ctrl-z) 

	string word;
	cin >> word;
	//loop ate palavra ser valida (palavra ou + ou ?)
	//se = ? (sugerir) , se = - (apagar) 
	if (Verify(posX, posY, direction, word))
		Insert(posX, posY, direction, word);
	else cout << "INVALIDO" << endl;

}
bool Board::Verify(int posX, int posY,char direction, string word)
{
	Cursor.moveTo(posX, posY, direction);
	int size = word.length();
	if (Cursor.CursorMainCoord() + size > BoardMainCoord())
		return false;
	if (Cursor.CursorMainCoord() > 0) //verifica se antes tem letra
	{
		Cursor--;
		if (ShowChar() != '#' && ShowChar() != '.')
			return false;
		Cursor++;
	}
	for (int i = 0; i < size; i++)
	{
		if (ShowChar() != word[i] && ShowChar() != '.')
			return false;
		Cursor++;
	}
	if (Cursor.CursorMainCoord() < BoardMainCoord()) //verifica se depois tem letra
		if (ShowChar() != '#' && ShowChar() != '.')
			return false;
	return true;
}

void Board::Insert(int posX, int posY, char direction, string word)
{
	Cursor.moveTo(posX, posY, direction);
	int size= word.length();
	if (Cursor.CursorMainCoord() > 0)
	{
		Cursor--;
		ChangeChar('#');
		Cursor++;
	}
	for (int i = 0; i<size; i++)
	{
		ChangeChar(word[i]);
		Cursor++;
	}
	if (Cursor.CursorMainCoord()<BoardMainCoord()) //verifica se pode por # depois
		ChangeChar('#');
}

void Board::ChangeChar(char letra)
{
	board[Cursor.x][Cursor.y] = letra;
}
char Board::ShowChar()
{
	return board[Cursor.x][Cursor.y];
}

int Board::BoardMainCoord()
{
	if (Cursor.dir == 'H')
		return x;
	else return y;
}
