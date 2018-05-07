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

void ShowVector(vector<string> v);

Board::Board(int size_x, int size_y, string dict_file) {
	dict.load(dict_file);
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	this->size_x = size_x;
	this->size_y = size_y;
}

Board::Board(int size_x, int size_y, Dictionary dict) {
	this->dict = dict;
	vector<vector<char>> v(size_x, vector<char>(size_y, '.'));
	board = v;
	this->size_x = size_x;
	this->size_y = size_y;
}

void Board::display() const {
	const size_t N_COLUMNS = size_x;
	const size_t N_ROWS = size_y;
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

void Board::Menu()
{
	string input_coords;
	string input_word;
	input_coords = inputCoords();
	input_word = inputWord();
	if (input_word == "-")
	{
		if (!Delete(input_coords)) // Tenta apagar
			cout << "No words start in that position" << endl;
	}
	else 
	if (input_word == "?")
		{
			vector<string> sugestoes = getSuggestions(input_coords);
			 if (sugestoes.empty())
			cout << "No words found in the dictionary" << endl;
			 else
			 {
				 cout << "Sugested Words: ";
				 ShowVector(sugestoes);
			 }
		}
		else
			if (!Verify(input_coords, input_word)) // Verifica se e valido e adiciona ao map de palavras
				cout << "Word does not fit in the board" << endl;
}
string Board::inputCoords()
{
	string input_coords;
	bool validCoords;
	do {
		validCoords = true;
		cout << "Position ( LCD / CTRL-Z = stop )  ? ";
		cin >> input_coords;
		if (cin.eof()) //CTRL-Z
		{
			cin.clear(); //NAO FUNCIONA SEM ISTO IDK WHY
			char decision;
			cout << "Save board? (y/n) "; 
			cin >> decision;
			toupper(decision);
			if (decision == 'Y')
			{
				saveFile("ola.txt");  //coisa com os numeros que eu nao sei !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				cout << "Board Saved." << endl;
			}
			exit(0);
		}
		if (cin.fail()) //input invalido
		{
			cin.clear();
			validCoords = false;
		}
		else
		{
			stringUpper(input_coords); input_coords[1] = tolower(input_coords[1]); // Converte todos os carateres para maiusculas menos o segundo. (Formato LcD).
			if (input_coords.size() != 3)
				validCoords = false;
			else 
				if(!(input_coords[0]>='A' && (int)input_coords[0] - 'A' < size_x && input_coords[1] >= 'a' && (int)(input_coords[1]) - 'a' < size_y && (input_coords[2] == 'H'|| input_coords[2] == 'V'))) //verifica se esta dentro dos limites 
				validCoords = false;
		}
		if (!validCoords)
			cout << "Invalid Position" << endl;
		cin.ignore(1000, '\n');
	} while (!validCoords);
	return input_coords;
}

string Board::inputWord()
{
	string input_word;
	bool validWord;
	do
	{
		validWord = true;
		cout << "Word ( - = remove / ? = help )    ? ";
		cin >> input_word;
		if (cin.fail()) //input invalido
		{
			cin.clear();
			cin.ignore(1000, '/n');
			validWord = false;
			cout << "Invalid Input" << endl;
		}
		else
		{
			stringUpper(input_word);
			if (!dict.wordExists(input_word) && input_word != "-" && input_word != "?") //EXISTE NO DICIONARIO?
			{
				cout << "Word does not exist in the dictionary" << endl;
				validWord = false;
			}
		}
		cin.ignore(1000, '\n');
	} while (!validWord);
	return input_word;
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

void Board::saveFile(string file_path) {
	ofstream file_dest(file_path);
	file_dest << file_path << endl << endl; // file_path no topo
	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++)
			file_dest << board[j][i] << ' ';
		file_dest << endl;
	}
	file_dest << endl;
	for (map<string, string>::const_iterator ite = placedWords_Coords.begin(); ite != placedWords_Coords.end(); ite++)
		file_dest << ite->first << " " << ite->second << endl;
}

void Board::loadFile(string file_path) {
	reset(MAX_SIZE, MAX_SIZE); //
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
			// file_orig.clear(); file_orig.ignore(1000, '\n'); // Acho que isto nao e preciso. Deixar estar caso haja problemas relacionados.
		}
	}
	else
		cout << "File could not be opened." << endl;
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

vector<string> Board::getSuggestions(string coords) { // Recebe coordenadas (LcD) e devolve todas as possibilidades para essa casa.
	vector<string> suggestions;
	Cursor.moveTo(coords);
	int wordSize = 1; // wordSize vai de 1 até ao limite do tabuleiro.
	for (int i = Cursor.MainCoord(); i < CoordLimit(); i++) {
		vector<string> wildcards = dict.getWildcardMatches(getWildcard(coords, wordSize)); // recebe as wildcards associadas ao wordSize atual, com as coordenadas fornecidas.
		suggestions.insert(suggestions.end(), wildcards.begin(), wildcards.end()); // Insere essas wildcards no vetor.
		wordSize++;
	}
	return suggestions;
}

string Board::getWildcard(string coords, int size) { //  Recebe coordenadas (LcD) e o tamanho da palavra e retorna uma string de '?' e letras, dependendo se a casa está preenchida ou nao.
	string wildcard;
	Cursor.moveTo(coords);
	if (Cursor.MainCoord() > 0) // Se puder "andar para trás"
	{
		Cursor--;
		cout << isalpha(ShowChar());
		if (isalpha(ShowChar())) // Se o anterior for uma letra.
			return "";
		Cursor++;
	}
	for (int i = 0; i < size; i++) { // Vê cada carater.
		if (isalpha(ShowChar()))
			wildcard.append(1, ShowChar()); // Adiciona a string de retorno o carater.
		else
			wildcard.append("?"); // Adiciona a string de retorno o carater '?'.
		Cursor++;
	}
	if (Cursor.MainCoord() < CoordLimit() && isalpha(ShowChar())) // Se o cursor conseguir avançar uma casa
		return ""; // E se o carater a seguir ao ultimo for uma letra.
	return wildcard;
}

void ShowVector(vector<string> v)
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << "  ";
	cout << endl;
}
