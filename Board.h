#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include "Cursor.h"
using namespace std;

class Board
{
public:
	Board();
	Board(int size_x, int size_y);
	map<string, string> getCoordsMap() const;
	size_t getBoardNumber() const;
	string getWildcard(string coords, size_t size);
	string getWord(string coords) const;
	map<string, string> extraWords();
	bool validCoords(string &coords);
	bool hasWord(string word) const;
	void display() const;
	bool Verify(string coords, string palavra);//Ve se pode por a palavra
	void Insert(string word, string coords);
	bool Delete(string coords);
	void RefreshBoard();
	void ChangeChar(char letra); //altera no board a letra com base na posicao do cursor
	char ShowChar() const; //ve que letra esta na posicao do cursor
	size_t CoordLimit(Cursor c) const; // dá a coordenada maxima do board na direcao atual do cursor (coordlimit() já está fora do tabuleiro)
	bool isFull() const;
	void clear();
	void reset();
	void reset(size_t size_x, size_t size_y);
	void shrinkBoard(size_t newSize_x, size_t newSize_y);
	void Fill();
	void grid();
	void loadFile(string file_path);
	string saveFile(string dict_path, bool is_StatFile = false);
	bool operator==(const Board& right);
	static const size_t MAX_SIZE = 20;
private:
	size_t boardNumber;
	size_t size_x, size_y;
	vector<vector<char>> board; // Mais usado visualmente.
	map<string, string> placedWords_Coords; // ["LCD", "Palavra"]. Guarda a informação mais necessária.
	set<string> blackCells_Coords;
	Cursor cursor;
	void Insert_in_board(string coords, string palavra); //mete
};

size_t find_BoardNumber();
