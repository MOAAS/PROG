#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "consolecolors.h"

Board::Board() {
	size_x = MAX_SIZE;
	size_y = MAX_SIZE;
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	this->boardNumber = find_BoardNumber();
}


Board::Board(int size_x, int size_y) {
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	this->size_x = size_x;
	this->size_y = size_y;
	this->boardNumber = find_BoardNumber();
}

void Board::display() const {
	const size_t N_COLUMNS = size_x;
	const size_t N_ROWS = size_y;
	char letras_cima = 'a'; // Limite superior terá letras minúsculas.
	char letras_lado = 'A'; // Limite inferior terá letras maiúsculas
	setcolor(LIGHTRED);
	cout << "   ";
	for (int i = 0; i < N_COLUMNS; i++) {
		cout << letras_cima << ' ';
		letras_cima++;
	}
	cout << endl;
	for (int i = 0; i < N_ROWS; i++) {
		cout << letras_lado << "  ";
		setcolor(BLACK, WHITE);
		letras_lado++;
		for (int j = 0; j < N_COLUMNS; j++)
			cout << board[j][i] << ' ';
		setcolor(LIGHTRED);
		cout << endl;
	}
	setcolor(WHITE);
}

bool Board::Verify(string coords, string word) // Verifica se word cabe nas coordenadas indicadas por coords (formato LcD).
{
    Cursor.moveTo(coords);
    size_t size = word.length();
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
    placedWords_Coords[coords] = word; // adiciona ao map
    RefreshBoard(); // atualiza o board
    return true;
}


bool Board::Delete(string coords) // Recebe uma string no formato LcD e apaga a palavra que começa nessa casa.
{
	if (placedWords_Coords.find(coords) == placedWords_Coords.end())
		return false;
	else
	{
		placedWords_Coords.erase(coords);
		RefreshBoard(); // atualiza o board
		return true;
	}
}

void Board::RefreshBoard() { // Vai ao map, coloca as palavras no tabuleiro
	clear();
	for (map<string, string>::iterator it = placedWords_Coords.begin(); it != placedWords_Coords.end(); ++it)
		Insert_in_board(it->first, it->second);
}

void Board::Insert_in_board(string coords, string word) // Insere word apenas no tabuleiro visual. coords indica onde a palavra começa, no formato LcD. 
{
	Cursor.moveTo(coords);
	size_t size = word.length();
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
}

void Board::ChangeChar(char letra) // Altera o char onde o cursor esta
{
	board[Cursor.x][Cursor.y] = letra;
}

char Board::ShowChar() const // Lê o char onde o cursor esta
{
	return board[Cursor.x][Cursor.y];
}

size_t Board::CoordLimit() const
{
	if (Cursor.dir == 'H')
		return size_x;
	else return size_y;
}

bool Board::isFull() const // Verifica se o tabuleiro encheu
{
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
			if (board[i][j] == '.')
				return false;
	}
	return true;
}

void Board::Fill() // Enche as casas vazias com '#'
{
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
			if (board[i][j] == '.')
				board[i][j] = '#';
	}
}

string Board::saveFile(string dict_path) {
	ostringstream oss;
	oss << "b" << setfill('0') << setw(3) << boardNumber;
	oss << ".txt"; // oss = bxxx.txt
	ofstream file_dest(oss.str());
	file_dest << dict_path << endl << endl; 
	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++)
			file_dest << board[j][i] << ' ';
		file_dest << endl;
	}
	file_dest << endl;
	for (map<string, string>::const_iterator ite = placedWords_Coords.begin(); ite != placedWords_Coords.end(); ite++)
		file_dest << ite->first << " " << ite->second << endl;
	return oss.str();
}

void Board::loadFile(string file_path) {
	this->boardNumber = stoi(file_path.substr(1, 3)); // file_path = bxxx.txt, substr(1, 3) = xxx. Converte para int.
	reset(MAX_SIZE, MAX_SIZE); 
	ifstream file_orig(file_path); string line;
	size_t sizeX_file, sizeY_file = 0;
	if (file_orig.is_open()) {
		getline(file_orig, line); // tira o nome do ficheiro.
		getline(file_orig, line); // tira a newline.
		getline(file_orig, line); // tira a primeira linha do tabuleiro.
		for (int i = 0; line != ""; i++) {
			sizeX_file = line.size() / 2; // Vai determinar o tamanho do tabuleiro no ficheiro.
			Cursor.moveTo(0, i);
			for (int j = 0; j < line.size(); j = j + 2) {
				ChangeChar(line[j]);
				Cursor++;
			}
			sizeY_file++;
			getline(file_orig, line);
		}
		size_x = sizeX_file; size_y = sizeY_file;
		shrinkBoard(sizeX_file, sizeY_file);
		string word, coords; // Coords na forma LcD!
		while (file_orig >> coords) {
			file_orig >> word;
			placedWords_Coords[coords] = word;
		}
	}
	else {
		cout << "File could not be opened." << endl;
		file_orig.close();
	}
}

void Board::shrinkBoard(size_t newSize_x, size_t newSize_y) {
	board.resize(newSize_x);
	for (int i = 0; i < board.size(); i++) {
		board[i].resize(newSize_y);
	}
}

void Board::clear() { // Apenas limpa o Board, nao altera o map com as palavras.
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
}


void Board::reset() {  // Reinicia o board (Apaga o tabuleiro e o conteudo do map)
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	placedWords_Coords.clear(); // Limpa o map
}

void Board::reset(size_t newSize_x, size_t newSize_y) { // Reinicia o board com um novo tamanho
	vector<vector<char>> v(newSize_x, vector<char>(newSize_y, '.'));
	board = v;
	this->size_x = newSize_x;
	this->size_y = newSize_y;
	placedWords_Coords.clear(); // Limpa o map
}

string Board::getWildcard(string coords, int size) { //  Recebe coordenadas (LcD) e o tamanho da palavra e retorna uma string de '?' e letras, dependendo se a casa está preenchida ou nao.
	string wildcard;
	Cursor.moveTo(coords);
	if (Cursor.MainCoord() > 0) // Se puder "andar para trás"
	{
		Cursor--;
		if (isalpha(ShowChar())) // Se o anterior for uma letra.
			return "";
		Cursor++;
	}
	for (int i = 0; i < size; i++) { // Vê cada carater.
		if (ShowChar() == '.')
			wildcard.append("?"); // Adiciona a string de retorno o carater '?'.
		else if (ShowChar() == '#')
			break;
		else if (isalpha(ShowChar())) // isalpha apenas por segurança, em princípio será sempre ser alfabetico
			wildcard.append(1, ShowChar()); // Adiciona a string de retorno o carater.
		Cursor++;
	}
	if (Cursor.MainCoord() < CoordLimit() && isalpha(ShowChar())) // Se o cursor conseguir avançar uma casa
		return ""; // E se o carater a seguir ao ultimo for uma letra.
	return wildcard;
}

bool Board::validCoords(string &coords) {
	stringUpper(coords);
	if (coords.size() != 3 || coords[0] < 'A' || coords[0] - 'A' >= size_y || coords[1] < 'A' || coords[1] - 'A' >= size_x || (coords[2] != 'H' && coords[2] != 'V'))
		return false;
	coords[1] = tolower(coords[1]); // Converte o segundo carater para minuscula (Formato LcD).
	return true;
}

string Board::getWord(string coords) const {
	for (auto ite : placedWords_Coords) {
		if (ite.first == coords)
			return ite.second;
	}
	return "";
}

bool Board::hasWord(string word) const {
	for (auto ite : placedWords_Coords) {
		if (ite.second == word)
			return true;
	}
	return false;
}

int find_BoardNumber() {
	// Para determinar o número de tabuleiros.
	size_t numBoards = 0;
	ostringstream oss;
	fstream f;
	do { // Este ciclo descobre o número de tabuleiros que existem
		f.close();
		oss.str("");
		oss << "b" << setfill('0') << setw(3) << numBoards+1;
		oss << ".txt"; // oss = bxxx.txt
		f.open(oss.str());
		numBoards++;
	} while (f.good());
	return numBoards;
}

