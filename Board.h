#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include "Cursor.h"
#include "Dictionary.h"
using namespace std;

class Board
{
public:
	Board(int size_x, int size_y, string dict_file);
	Board(int size_x, int size_y, Dictionary dict);
	void display() const;
	void Menu();
	string inputCoords();
	string inputWord();
	bool Verify(string coords, string palavra);//Ve se pode por a palavra
	void Insert_in_board(string coords, string palavra);//mete
	void RefreshBoard();
	void shrinkBoard(size_t newSize_x, size_t newSize_y);
	bool Delete(string coords);
	void ChangeChar(char letra); //altera no board a letra com base na posicao do cursor
	char ShowChar() const; //ve que letra esta na posicao do cursor
	size_t CoordLimit() const; // dá a coordenada maxima do board na direcao atual do cursor (coordlimit() já está fora do tabuleiro)
	bool isFull() const;
	void clear();
	void reset();
	void reset(size_t size_x, size_t size_y);
	void Fill();
	void saveFile(string file_path);
	void loadFile(string file_path);
	string getWildcard(string coords, int size);
	vector<string> getSuggestions(string coords);
private:
	vector<vector<char>> board; // Apenas usado visualmente.
	size_t size_x, size_y;
	map<string, string> placedWords_Coords; // ["LCD", "Palavra"]. É aqui que a informação realmente está.
	Cursor Cursor;
	Dictionary dict;
	static const size_t MAX_SIZE = 20;
};
	Dictionary dict;
	static const size_t MAX_SIZE = 20;
};
