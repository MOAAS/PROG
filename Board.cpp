#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <fstream>
#include "consolecolors.h"
#include "Dictionary.h"

Board::Board(int size_x, int size_y) {
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	this->size_x = size_x;
	this->size_y = size_y;
}

void Board::display() const {
	const int N_COLUMNS = size_x;
	const int N_ROWS = size_y;
	char letras_cima = 'a'; // Limite superior terá letras minúsculas.
	char letras_lado = 'A'; // Limite inferior terá letras maiúsculas
	setcolor(LIGHTRED);
	cout << "   ";
	for (int i = 0; i < N_ROWS; i++) {
		cout << letras_cima << ' ';
		letras_cima++;
	}
	cout << endl;
	for (int i = 0; i < N_COLUMNS; i++) {
		cout << letras_lado << "  ";
		setcolor(BLACK, WHITE);
		letras_lado++;
		for (int j = 0; j < N_ROWS; j++)
			cout << board[j][i] << ' ';
		setcolor(LIGHTRED);
		cout << endl;
	}
	setcolor(WHITE);
}

void Board::getWord()
{
	int posX, posY;
	char direction;
	//CUIDADOS COM INPUT 
	// '-' e '?'
	string input;
	cin >> input; stringUpper(input); input[1] = tolower(input[1]); // Converte todos os carateres para maiusculas menos o segundo.
	posY = (int)input[0] - (int)'A';
	posX = (int)input[1] - (int)'a';
	direction = input[2];
	//loop ate input ser valido (coordenadas dentro dos limites ou ctrl-z) 

	string word;
	cin >> word;
	stringUpper(word);
	//loop ate palavra ser valida (palavra ou + ou ?)
	//se = ? (sugerir) , se = - (apagar) 
	if (Verify(posX, posY, direction, word))
		Insert(posX, posY, direction, word);
	else cout << "INVALIDO" << endl;
}
bool Board::Verify(int posX, int posY, char direction, string word)
{
	Cursor.moveTo(posX, posY, direction);
	int size = word.length();
	if (Cursor.MainCoord() + size > CoordLimit())
		return false;
	if (Cursor.MainCoord() > 0) //verifica se antes tem letra
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
	if (Cursor.MainCoord() < CoordLimit()) //verifica se depois tem letra
		if (ShowChar() != '#' && ShowChar() != '.')
			return false;
	return true;
}

void Board::Insert(int posX, int posY, char direction, string word)
{
	Cursor.moveTo(posX, posY, direction);
	int size = word.length();
	if (Cursor.MainCoord() > 0)
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
	if (Cursor.MainCoord() < CoordLimit()) //verifica se pode por # depois
		ChangeChar('#');
	char LinColDir[4] = { posY + 'A', posX + 'a', direction, '\0' }; // LinColDir é uma string de 3 carateres que indica a linha a coluna e direçao da palavra.
	placedWordsCoords[LinColDir] = word;
}

void Board::ChangeChar(char letra)
{
	board[Cursor.x][Cursor.y] = letra;
}
char Board::ShowChar() const
{
	return board[Cursor.x][Cursor.y];
}

int Board::CoordLimit() const
{
	if (Cursor.dir == 'H')
		return size_x;
	else return size_y;
}

vector<string> Board::getPlacedWords() const {
	vector<string> placedWords;
	for (map<string, string>::const_iterator ite = placedWordsCoords.begin(); ite != placedWordsCoords.end(); ite++) {
		placedWords.push_back(ite->second);
	}
	return placedWords;
}

bool Board::isFull() const
{
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
			if (board[i][j] == '.')
				return false;
	}
	return true;
}

void Board::Fill()
{
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
			if (board[i][j] == '.')
				board[i][j] = '#';
	}
}

void Board::saveFile(string file_path) {
	ofstream file_dest(file_path);
	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++)
			file_dest << board[j][i] << ' ';
		file_dest << endl;
	}
	file_dest << endl << "WORD COORDINATES:" << endl;
	for (map<string, string>::const_iterator ite = placedWordsCoords.begin(); ite != placedWordsCoords.end(); ite++)
		file_dest << ite->first << " " << ite->second << endl;
}

void Board::loadFile(string file_path) {
	ifstream file_orig(file_path); string line;
	int size_x_file, size_y_file = -1; // Começa a -1 pois a linha extra abaixo do board incrementa o tamanho em 1 a mais.
	if (file_orig.is_open()) {
		getline(file_orig, line);
		size_x_file = line.size() / 2; // 
		while (line != "WORD COORDINATES:") {
			getline(file_orig, line);
			size_y_file++;
		}
		clear(size_x_file, size_y_file);
		string word, coords; // Coords na forma LCD
		while (file_orig >> coords) {
			file_orig >> word;
			Insert(LCDtoPosX(coords), LCDtoPosY(coords), LCDtoDir(coords), word);
			// file_orig.clear(); file_orig.ignore(1000, '\n'); // Acho que isto nao e preciso. Deixar estar caso haja problemas relacionados.
		}
	}
}

void Board::clear() {
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	placedWordsCoords.clear(); // limpa o map
}

void Board::clear(int new_size_x, int new_size_y) {
	vector<vector<char>> v(new_size_x, vector<char>(new_size_y, '.'));
	board = v;
	this->size_x = new_size_x;
	this->size_y = new_size_y;
	placedWordsCoords.clear(); // limpa o map
}


int LCDtoPosX(string LCD) {
	return LCD[1] - 'a';
}

int LCDtoPosY(string LCD) {
	return LCD[0] - 'A';
}

char LCDtoDir(string LCD) {
	return LCD[2];
}
