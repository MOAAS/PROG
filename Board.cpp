#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <algorithm>
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

/*
Mostra no ecrã o tabuleiro.

const size_t N_COLUMNS - tamanho do tabuleiro na horizontal
const size_t N_ROWS - tamanho do tabuleiro na vertical
char letras_cima - começa em 'a' e vai aumentando 1
char letras_lado - começa em 'A' e vai aumentando 1
*/
void Board::display() const {
	cout << endl;
	const size_t N_COLUMNS = size_x;
	const size_t N_ROWS = size_y;
	char letras_cima = 'a'; // Limite superior terá letras minúsculas.
	char letras_lado = 'A'; // Limite inferior terá letras maiúsculas
	setcolor(LIGHTRED);
	cout << "   ";
	// a b c d ...
	for (int i = 0; i < N_COLUMNS; i++) {
		cout << letras_cima << ' ';
		letras_cima++;
	}
	cout << endl;
	for (int i = 0; i < N_ROWS; i++) {
		cout << letras_lado << "  "; // A, B, C, D ...
		setcolor(BLACK, WHITE);
		letras_lado++;
		for (int j = 0; j < N_COLUMNS; j++) {
			if (board[j][i] == '#')
				setcolor(LIGHTGRAY, BLACK);
			cout << board[j][i];
			setcolor(BLACK, WHITE);
			cout << " ";
		}
		setcolor(LIGHTRED);
		cout << endl;
	}
	setcolor(WHITE);
} 

/* 
Verifica se word cabe nas coordenadas indicadas por coords (formato LcD). Verifica se tem alguma letra antes, depois, e se cabe no tabuleiro.

size_t size - tamanho da palavra
*/
bool Board::Verify(string coords, string word) 
{
	cursor.moveTo(coords);
	size_t size = word.length();
	if (cursor.MainCoord() + size > CoordLimit(cursor))
		return false;
	if (cursor.MainCoord() > 0) //verifica se antes tem letra
	{
		cursor--;
		if (ShowChar() != '#' && ShowChar() != '.')
			return false;
		cursor++;
	}
	for (int i = 0; i < size; i++) // percorre a linha/coluna
	{
		if (ShowChar() != word[i] && ShowChar() != '.') // se a casa estiver ocupada com um carater que não seja o da palavra
			return false;
		cursor++;
	}
	if (cursor.MainCoord() < CoordLimit(cursor)) //verifica se depois tem letra
		if (ShowChar() != '#' && ShowChar() != '.')
			return false;
	return true;
}

void Board::Insert(string word, string coords) {
	placedWords_Coords[coords] = word; // adiciona ao map
	RefreshBoard(); // atualiza o board
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

// Limpa o tabuleiro. Vai ao map, coloca as palavras no tabuleiro de novo.
void Board::RefreshBoard() {
	clear();
	for (map<string, string>::iterator it = placedWords_Coords.begin(); it != placedWords_Coords.end(); ++it)
		Insert_in_board(it->first, it->second);
	for (set<string>::iterator it = blackCells_Coords.begin(); it != blackCells_Coords.end(); ++it)
	{
		cursor.moveTo(*it);
		ChangeChar('#');
	}
}

// Insere word apenas no tabuleiro visual. coords indica onde a palavra começa, no formato LcD. 
void Board::Insert_in_board(string coords, string word) 
{
	cursor.moveTo(coords);
	size_t size = word.length();
	if (cursor.MainCoord() > 0) // MainCoord = valor da coordenada na direção do cursor
	{
		cursor--;
		ChangeChar('#');
		cursor++;
	}
	for (int i = 0; i<size; i++)
	{
		ChangeChar(word[i]);
		cursor++;
	}
	if (cursor.MainCoord() < CoordLimit(cursor)) //verifica se pode por # depois
		ChangeChar('#');
}

void Board::ChangeChar(char letra) // Altera o char onde o cursor esta
{
	board[cursor.x][cursor.y] = letra;
}

char Board::ShowChar() const // Lê o char onde o cursor esta
{
	return board[cursor.x][cursor.y];
}

size_t Board::CoordLimit(Cursor c) const
{
	if (c.dir == 'H')
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

void Board::Fill()
{
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
			if (board[i][j] == '.')
				board[i][j] = '#';
	}
}

/*
Guarda em primeiro lugar o board, depois o map.

ostringstream oss - auxiliar para obter o nome do ficheiro.
ofstream file_dest - ficheiro onde se guarda);
*/
string Board::saveFile(string dict_path) {
	ostringstream oss;
	oss << "b" << setfill('0') << setw(3) << boardNumber;
	oss << ".txt"; // oss = bxxx.txt
	ofstream file_dest(oss.str());
	file_dest << dict_path << endl << endl;
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++)
			file_dest << board[j][i] << ' ';
		file_dest << endl;
	}
	file_dest << endl;
	// Agora guarda-se o map.
	for (map<string, string>::const_iterator ite = placedWords_Coords.begin(); ite != placedWords_Coords.end(); ite++)
		file_dest << ite->first << " " << ite->second << endl;
	return oss.str();
}

/*
Carrega um ficheiro com um tabuleiro. Vai descobrindo o tamanho do tabuleiro à medida que vai lendo o ficheiro. Primeiro enche o tabuleiro, depois o map de coordenadas.
ifstream file_orig(file_path) - ficheiro de tabuleiro
string line - guarda cada linha do ficheiro
size_t sizeX_file, sizeY_file - guarda-se aqui o tamanho que o programa determina.
*/
void Board::loadFile(string file_path) {
	this->boardNumber = stoi(file_path.substr(1, 3)); // file_path = bxxx.txt, substr(1, 3) = xxx. Converte para int.
	reset(MAX_SIZE, MAX_SIZE); // Para não causar problemas de limites de vetor.
	ifstream file_orig(file_path); string line;
	size_t sizeX_file, sizeY_file = 0;
	if (file_orig.is_open()) {
		getline(file_orig, line); // tira o nome do ficheiro.
		getline(file_orig, line); // tira a newline.
		getline(file_orig, line); // tira a primeira linha do tabuleiro.
		// Enche-se o board primeiro
		for (int i = 0; line != ""; i++) {
			sizeX_file = line.size() / 2; // Vai determinar o tamanho do tabuleiro no ficheiro.
			cursor.moveTo(0, i); // move o cursor
			for (int j = 0; j < line.size(); j = j + 2) {
				ChangeChar(line[j]);
				cursor++;
			}
			sizeY_file++; // Por cada linha soma-se 1.
			getline(file_orig, line);
		}
		this->size_x = sizeX_file; // tamanho de uma linha / 2
		this->size_y = sizeY_file; // numero de linhas
		shrinkBoard(sizeX_file, sizeY_file); 
		// Enche-se o MAP agora
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

void Board::clear() { 
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
}


void Board::reset() {  
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	placedWords_Coords.clear(); // Limpa o map
}

void Board::reset(size_t newSize_x, size_t newSize_y) { 
	vector<vector<char>> v(newSize_x, vector<char>(newSize_y, '.'));
	board = v;
	this->size_x = newSize_x;
	this->size_y = newSize_y;
	placedWords_Coords.clear(); // Limpa o map
}

/*
Recebe coordenadas (LcD) e o tamanho da palavra e retorna uma string de '?' e letras, dependendo se a casa está preenchida ou nao. Cada '.' fica um '?' cada letra fica a letra.

string wildcard - a devolver
*/
string Board::getWildcard(string coords, size_t size) { 
	string wildcard;
	cursor.moveTo(coords);
	if (cursor.MainCoord() > 0) // Se puder "andar para trás"
	{
		cursor--;
		if (isalpha(ShowChar())) // Se o anterior for uma letra.
			return "";
		cursor++;
	}
	for (int i = 0; i < size; i++) { // Vê cada carater.
		if (ShowChar() == '.')
			wildcard.append("?"); // Adiciona a string de retorno o carater '?'.
		else if (ShowChar() == '#')
			return "";
		else wildcard.append(1, ShowChar()); // Adiciona à string de retorno o carater.
		cursor++;
	}
	if (cursor.MainCoord() < CoordLimit(cursor) && isalpha(ShowChar())) // Se o cursor conseguir avançar uma casa
		return "";                                                      // E se o carater a seguir ao ultimo for uma letra.
	return wildcard;
}

// Coordenadas têm de respeitar tambem o tamanho do tabuleiro. Se forem válidas converte mara maiúscula exceto o 2º carater
bool Board::validCoords(string &coords) {
	transform(coords.begin(), coords.end(), coords.begin(), ::toupper); // Converte para letras maiusculas!
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

map<string, string> Board::extraWords() {
	map<string, string> newWords;
	bool gettingWord = false;
	string extraWord;
	string extraWordCoords;
	string CoordsLCD;
	for (size_t i = 0; i<size_x; i++) {
		for (size_t j = 0; j < size_y; j++) {
			CoordsLCD = { (char)(j + 'A'), (char)(i + 'a'), 'V' };
			if (gettingWord) {  //ve se esta a meio de uma palavra
				if (board[i][j] == '#' || board[i][j] == '.' || j == 0) { //palavra ja acabou?
					if (extraWord.size() > 1) 	//evita quando e so uma letra				
						newWords[extraWordCoords] = extraWord;		//adiciona ao map de palavras
					if (j == 0)	j--;
					gettingWord = false;
				}
				else extraWord += board[i][j]; // adiciona letra nova a palavra
			}
			else if (isalpha(board[i][j])) {//quando nao esta a meio da formacao de uma palavra. verifica se e uma letra
				if (placedWords_Coords.find(CoordsLCD) == placedWords_Coords.end()) { // se nao for o inicio de uma palavra do map  
					extraWord = board[i][j];                                          // de palavras , comeca uma nova palavra
					extraWordCoords = CoordsLCD;
					gettingWord = true;
				}
				else j += placedWords_Coords[CoordsLCD].size();	 //se for o inicio de uma palavra do map
			}
		}
	}
	//A MESMA COISA MAS HORIZONTAL
	for (size_t i = 0; i<size_y; i++) { //Diferenca (size_y)
		for (size_t j = 0; j < size_x; j++) {   //Diferenca (size_x)
			CoordsLCD = { (char)(i + 'A'), (char)(j + 'a'), 'H' }; // Diferenca (i,j,H)
			if (gettingWord) {
				if (board[j][i] == '#' || board[j][i] == '.' || j == 0) {  // Diferenca (j,i)
					if (extraWord.size() > 1)
						newWords[extraWordCoords] = extraWord;
					if (j == 0)	j--;
					gettingWord = false;
				}
				else extraWord += board[j][i]; // Diferenca (j,i)
			}
			else if (isalpha(board[j][i])) {//verifica se e uma letra
				if (placedWords_Coords.find(CoordsLCD) == placedWords_Coords.end()) {
					extraWord = board[j][i];
					extraWordCoords = CoordsLCD;
					gettingWord = true;
				}
				else j += placedWords_Coords[CoordsLCD].size();
			}
		}
	}
	return newWords;
}

void Board::grid() {
	string CoordsLCD; 
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			CoordsLCD = { (char)(i + 'A'), (char)(j + 'a'), 'H' }; // Do tipo "AaH"
			if (board[j][i] == '#')
				blackCells_Coords.insert(CoordsLCD); // Adiciona às coordenadas dos '#'
		}
	}
	reset(); // Limpa
	RefreshBoard(); // atualiza
}

map<string, string> Board::getCoordsMap() const {
	return placedWords_Coords;
}

size_t Board::getBoardNumber() const
{
	return boardNumber;
}

bool Board::operator==(const Board & right) {
	return board == right.board;
}

/*
Vai verificando se existe b001.txt, b002.txt, etc. até não existir, descobrindo assim o número do tabuleiro que se acabou de criar.

size_t numBoards - começa a 0, vai-se somando 1 cada vez que se tenta abrir um ficheiro
ostringstream oss - necessário para criar o nome do ficheiro: "bxxx.txt"
*/
size_t find_BoardNumber() {
	// Para determinar o número de tabuleiros.
	size_t numBoards = 0;
	ostringstream oss;
	fstream f;
	do { // Este ciclo descobre o número de tabuleiros que existem
		f.close();
		oss.str("");
		oss << "b" << setfill('0') << setw(3) << numBoards + 1;
		oss << ".txt"; // oss = bxxx.txt
		f.open(oss.str());
		numBoards++;
	} while (f.good());
	return numBoards;
}

// Apaga temporariamente a palavra que está nas coordenadas. Se a nova palavra couber nela, retorna true. Se não, false.
bool Board::replaceable(string coords, string oldword, string newword)
{
	Delete(coords);
	if (!Verify(coords, newword))
	{
		Insert(oldword, coords);
		return false;
	}
	Insert(oldword, coords);
	return true;
}
