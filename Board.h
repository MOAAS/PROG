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
	map<string, string> getCoordsMap() const; // devolve placedWords_Coords
	size_t getBoardNumber() const; // devolve boardNumber
	string getWildcard(string coords, size_t size); 
	string getWord(string coords) const; // devolve a palavra nas coordenas definidas por coords
	map<string, string> extraWords(); // devolve um map com todas as palavras formadas acidentalmente por outras que foram colocadas
	bool validCoords(string &coords); // determina se coordenadas são válidas
	bool hasWord(string word) const; // determina se já tem a palavra word
	void display() const; // mostra no ecrã
	bool Verify(string coords, string palavra); // vê se pode por a palavra em coords
	void Insert(string word, string coords); // insere word em coords
	bool Delete(string coords); // apaga a palavra em coords
	void RefreshBoard(); // atualiza o tabuleiro visual (baseado nas palavras do map)
	bool replaceable(string coords, string oldword, string newword); // verifica se dá para substituir, em coords, oldword por newword.
	void ChangeChar(char letra); //altera no board a letra com base na posicao do cursor
	char ShowChar() const; //ve que letra esta na posicao do cursor
	size_t CoordLimit(Cursor c) const; // dá a coordenada maxima do board na direcao atual do cursor (coordlimit() já está fora do tabuleiro!)
	bool isFull() const; // determina se o board está cheio
	void clear(); // Apenas limpa o tabuleiro, nao altera o map com as palavras.
	void reset(); // Reinicia o board (Apaga o tabuleiro e o conteudo do map)
	void reset(size_t size_x, size_t size_y); // Reinicia o board com um novo tamanho
	void shrinkBoard(size_t newSize_x, size_t newSize_y); // reduz o tamanho do board
	void Fill(); // enche os espaços vazios com '#'
	void grid(); // enche o set blackCells_coords e deixa estar apenas os '#'
	void loadFile(string file_path); // carrega o board de um ficheiro
	string saveFile(string dict_path); // guarda o board num ficheiro
	bool operator==(const Board& right); // compara apenas os tabuleiros visuais
	static const size_t MAX_SIZE = 20;
private:
	size_t boardNumber;
	size_t size_x, size_y; // largura e altura
	vector<vector<char>> board; // Mais usado visualmente.
	map<string, string> placedWords_Coords; // ["LCD", "Palavra"]. Guarda a informação mais necessária.
	set<string> blackCells_Coords; // guarda os '#'. usado apenas para o cwplayer
	Cursor cursor; // auxilia na construção do tabuleiro. Move-se pelo tabuleiro, lê as letras e altera-as.
	void Insert_in_board(string coords, string palavra); // insere apenas no tabuleiro visual.
};

size_t find_BoardNumber(); 
