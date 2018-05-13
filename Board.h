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
	Board();
	Board(int size_x, int size_y);
	void display() const;
	bool Verify(string coords, string palavra);//Ve se pode por a palavra
	void Insert(string word, string coords);
	void RefreshBoard();
	void shrinkBoard(size_t newSize_x, size_t newSize_y);
	bool Delete(string coords);
	void ChangeChar(char letra); //altera no board a letra com base na posicao do cursor
	char ShowChar() const; //ve que letra esta na posicao do cursor
	size_t CoordLimit(Cursor c) const; // dá a coordenada maxima do board na direcao atual do cursor (coordlimit() já está fora do tabuleiro)
	bool isFull() const;
	void clear();
	void reset();
	void reset(size_t size_x, size_t size_y);
	void Fill();
	string saveFile(string dict_path);
	void loadFile(string file_path);
	string getWildcard(string coords, int size);
	bool validCoords(string &coords);
	string getWord(string coords) const;
	bool hasWord(string word) const;
	map<string, string> extraWords();
	static const size_t MAX_SIZE = 20;
private:
	int boardNumber;
	size_t size_x, size_y;
	vector<vector<char>> board; // Mais usado visualmente.
	map<string, string> placedWords_Coords; // ["LCD", "Palavra"]. Guarda a informação mais necessária.
	Cursor cursor;
	void Insert_in_board(string coords, string palavra); //mete
};

int find_BoardNumber();
