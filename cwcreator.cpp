#include "stdafx.h"
#include "dictionary.h"
#include "consolecolors.h"
#include "board.h"
#include "Cursor.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <iomanip>
#include <utility>
using namespace std;

string displayInstructions();
bool restartCreator();

pair<Board, Dictionary> createPuzzle();
pair<Board, Dictionary> resumePuzzle();

string getInput_Coords(Board b1, Dictionary d1);
string getInput_Word(Board b1, Dictionary d1);

int boardBuilding(Board &b1, Dictionary d1);

void delete_at(string coords, Board &b1);
void displaySuggestions(Board b1, Dictionary d1, string coords);
void insert_at(string coords, string word, Board &b1);

void ShowVector(vector<string> v);

int main() {
	Board b1;
	Dictionary d1;
	pair<Board, Dictionary> Puzzle;
	while (true) { // Temporario.
		string option;
		do {
			clrscr();
			option = displayInstructions();
		} while (option != "0" && option != "1" && option != "2");
		if (option == "0")
			exit(0);
		else if (option == "1")
			Puzzle = createPuzzle();
		else if (option == "2")
			Puzzle = resumePuzzle();
		b1 = Puzzle.first;
		d1 = Puzzle.second;
		int code;
		do {
			code = boardBuilding(b1, d1);
			if (code == -1)
				cout << "The board still has some invalid words, it cannot be finished until those are removed." << endl;
		} while (code == -1);
		if (code) { // = 1, ou seja está bom!
			string newBoard_path = b1.saveFile(d1.filePath);
			cout << "The board is finished!" << endl;
			cout << "Board Saved, as " << newBoard_path << ".\n";
			if (restartCreator()) {
				clrscr();
				return 0;
			}
		}
		else { // = 0, ou seja tem de se recomeçar.
			clrscr();
			continue;
		}
	}
}

int boardBuilding(Board &b1, Dictionary d1) {
	bool user_isDone = false;
	while (!b1.isFull()) {
		string input_coords = getInput_Coords(b1, d1); // devolve string vazia caso haja Ctrl+Z
		if (input_coords == "") {
			user_isDone = true;
			break;
		}
		string input_word = getInput_Word(b1, d1);
		if (input_word == "-")
			delete_at(input_coords, b1);
		else if (input_word == "?")
			displaySuggestions(b1, d1, input_coords);
		else
			insert_at(input_coords, input_word, b1);
		cout << endl;
	}
	if (user_isDone) {
		if (restartCreator()) {
			clrscr();
			return 0;
		}
		else exit(EXIT_SUCCESS);
	}
	/*else if (b1.extraWords()) // tambem altera b1.
		return 1; // Está bom!
	else
		return -1; // Tem palavras invalidas*/
	return 1;
}

string displayInstructions() {
	cout << "=========================" << endl;
	cout << "INSTRUCTIONS:" << endl;
	cout << "You will be asked to input a position and its respective word. until you don't want to add any extra words to the board." << endl;
	cout << "Position (LCD / Ctrl-Z = stop)" << endl;
	cout << " LCD stands for Line Column and Direction.\n Line and Column will be a letter (limit depends on the size of the board) and Direction will either be 'H' (for Horizontal) or 'V' (for vertical).\n Using Ctrl+Z will finish the board creation." << endl;
	cout << "Word ( - = remove / ? = help)" << endl;
	cout << " If you input an invalid word you will be required to write something else, until your input is valid.\n If you type '-' the word starting from that position will be removed.\n Typing '?' will display every possible insertion." << endl;
	cout << "-------------------------" << endl << endl;
	cout << "OPTIONS:" << endl;
	cout << "1 - Create puzzle" << endl;
	cout << "2 - Resume puzzle" << endl;
	cout << "0 - Exit" << endl << endl;
	cout << "Option ? ";
	string option; getline(cin, option); cin.clear();
	return option;
}

pair<Board, Dictionary> createPuzzle() {
	string dictFile_path; ifstream file;
	int boardSizeX, boardSizeY;
	cout << "--------------------" << endl;
	cout << "CREATE PUZZLE" << endl;
	cout << "--------------------" << endl;
	do {
		cout << "Dictionary file name ? "; cin >> dictFile_path; cin.clear(); cin.ignore(10000, '\n');
		file.open(dictFile_path);
	} while (!file.is_open());
	file.close();
	Dictionary d1(dictFile_path);
	do {
		cout << "Board size (lines columns) ? "; cin >> boardSizeY >> boardSizeX; cin.clear(); cin.ignore(1000, '\n');// SizeY = numero de linhas. SizeX = numero de colunas !
	} while (boardSizeX < 0 || boardSizeY < 0 || boardSizeX > Board::MAX_SIZE || boardSizeY > Board::MAX_SIZE);
	cout << endl << endl;
	Board b1(boardSizeX, boardSizeY);
	pair<Board, Dictionary> BoardDict_Pair(b1, d1);
	return BoardDict_Pair;
}

pair<Board, Dictionary> resumePuzzle() {
	string boardFile_path, dictFile_path;
	ifstream file;
	cout << "--------------------" << endl;
	cout << "RESUME PUZZLE" << endl;
	do {
		file.close();
		cout << "File name ? "; cin >> boardFile_path; cin.clear(); cin.ignore(10000, '\n');
		file.open(boardFile_path);
	} while (!file.good());
	getline(file, dictFile_path); // tira a primeira linha!
	Dictionary d1(dictFile_path);
	Board b1; b1.loadFile(boardFile_path);
	pair<Board, Dictionary> BoardDict_Pair(b1, d1);
	return BoardDict_Pair;
}

string getInput_Coords(Board b1, Dictionary d1) {
	string input_coords;
	bool validCoords;
	do {
		b1.display(); cout << endl;
		validCoords = true;
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> input_coords;
		if (cin.eof()) {
			cin.clear();
			char decision;
			do {
				cout << "Save board to resume later (S) or finish creation (F) ? ";
				cin >> decision; cin.clear(); cin.ignore(10000, '\n');
				decision = toupper(decision);
				if (decision == 'F') {
					cout << "Creation is finished. ";
					b1.Fill();
				}
			} while (decision != 'S' && decision != 'F');
			string newBoard_path = b1.saveFile(d1.filePath);
			cout << "Board Saved, as " << newBoard_path << ".\n";
			return "";
		}
		else if (!b1.validCoords(input_coords)) // se forem validas tambem converte o necessario para minusculas e maiusculas.
			validCoords = false;
		if (!validCoords)
			cout << "Invalid Position" << endl;
		cin.ignore(1000, '\n');
	} while (!validCoords);
	return input_coords;
}

string getInput_Word(Board b1, Dictionary d1) {
	string input_word;
	bool validWord;
	do	{
		validWord = true;
		cout << "Word ( - = remove / ? = help ) ? ";
		cin >> input_word;
		stringUpper(input_word);
		if (!d1.wordExists(input_word) && input_word != "-" && input_word != "?") { //EXISTE NO DICIONARIO?
			cout << "The word " << input_word << " does not exist in the dictionary!" << endl;
			validWord = false;
		}
		else if (d1.wordExists(input_word) && !b1.hasWord(input_word)) {
			cout << "The word " << input_word << " is already in the board!" << endl;
			validWord = false;
		}			
		cin.ignore(1000, '\n');
	} while (!validWord);
	return input_word;
}

bool restartCreator() {
	char decision; 
	cout << endl;
	do {
		cout << "Do you want to restart? (Y/N) ";
		cin >> decision; cin.clear(); cin.ignore(10000, '\n');
		decision = toupper(decision);
	} while (decision != 'Y' && decision != 'N');
	if (decision == 'Y')
		return true;
	else exit(EXIT_SUCCESS);
}

void delete_at(string coords, Board &b1) {
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << b1.getWord(coords) << ".\n";
	else
		cout << "No words start in that position." << endl;
}

void displaySuggestions(Board b1, Dictionary d1, string coords) {
	vector<string> sugestoes;
	Cursor c1; c1.moveTo(coords);
	int wordSize = 1; // wordSize vai de 1 até ao limite do tabuleiro.
	cout << "Fecthing suggested words..." << endl;
	for (int i = c1.MainCoord(); i < b1.CoordLimit(); i++) {
		string wildcard = b1.getWildcard(coords, wordSize);
		vector<string> wildcards = d1.getWildcardMatches(wildcard); // recebe as wildcards associadas ao wordSize atual, com as coordenadas fornecidas.
		sugestoes.insert(sugestoes.end(), wildcards.begin(), wildcards.end()); // Insere essas wildcards no vetor.
		wordSize++;
	}
	if (sugestoes.empty())
		cout << "No words found in the dictionary" << endl;
	else {
		cout << "Suggested Words: ";
		ShowVector(sugestoes);
	}
}

void insert_at(string coords, string word, Board &b1) {
	if (b1.Verify(coords, word)) {
		cout << "The word " << word << " has been inserted." << endl;
	}
	else
		cout << "The word " << word << " does not fit in the board." << endl;
}

void ShowVector(vector<string> v)
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << "  ";
	cout << endl;
}
