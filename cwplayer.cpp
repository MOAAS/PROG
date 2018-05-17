#include "stdafx.h"
#include "dictionary.h"
#include "consolecolors.h"
#include "board.h"
#include "player.h"
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
#include <ctime>
using namespace std;

typedef map<string, vector<string>> cluemap; // ["AaH", "help"]
typedef bool flag;

int getColor(string coords, Board solutionBoard, Board emptyBoard);
void displayClues(cluemap clues, Board solutionBoard, Board emptyBoard, bool showCorrectGuesses);
bool addRandomClue(string coords, Board solutionBoard, cluemap &clues, Dictionary dict);
cluemap loadRandomClues(Board solutionBoard, Dictionary dict);

void delete_at(string coords, Board &b1);
bool replaceable(string coords, string oldword, string newword, Board emptyBoard);

string getInput_PlayerName();
string getInput_Coords(Board solutionBoard);
string getInput_Word(Board emptyBoard, Board solutionBoard, string coords);

string openBoard(Board &solutionBoard, Board &emptyBoard);
void addExtraWords(Board &b1);
void clearBadInput();
void saveStats(Board solutionB1, Dictionary dict, Player p1, size_t timeTaken);

void displayInstructions();
void restartPlayer();
bool getInput_showCorrectGuesses();
bool getInput_Difficulty();

int main() {
	srand((unsigned int)time(NULL));
	while (true) {
		Board solutionB1, b1; // b1 = board a ser resolvido.
		Dictionary d1;
		Player p1;
		displayInstructions();
		p1.setName(getInput_PlayerName());
		string dictFile_path = openBoard(solutionB1, b1);
		d1.load(dictFile_path);
		cluemap clues = loadRandomClues(solutionB1, d1);
		flag boardSolved = false;
		bool easyMode = getInput_Difficulty();
		bool showCorrectGuesses = false;
		cout << endl << "Clock starts now!" << endl;
		p1.startClock();
		do {
			b1.display();
			displayClues(clues, solutionB1, b1, showCorrectGuesses || easyMode);
			showCorrectGuesses = false;
			string coords = getInput_Coords(solutionB1);
			string word = getInput_Word(b1, solutionB1, coords);
			if (word == "-")
				delete_at(coords, b1);
			else if (word == "?") {
				if (addRandomClue(coords, solutionB1, clues, d1))
					p1.incClues();
				else cout << "No more clues for that word!" << endl;
			}
			else if (word == "") // ctrl + z 
				continue; // proximo ciclo
			else b1.Insert(word, coords);
			// BOARD CHEIO!
			if (b1.isFull()) {
				addExtraWords(b1);
				b1.display();
				if (solutionB1 == b1) // == compara os boards!
					boardSolved = true;
				else if (getInput_showCorrectGuesses()) {
					p1.incClues();
					showCorrectGuesses = true;					
				}
			}

		} while (!boardSolved);
		size_t timeTaken = p1.endClock();
		cout << "Congratulations!" << endl;
		saveStats(solutionB1, d1, p1, timeTaken);
		restartPlayer(); // Pergunta ao utilizador se quer recomeçar. limpa a consola se for para reiniciar.
	}
}

string openBoard(Board &solutionBoard, Board &emptyBoard) { // retorna o nome do ficheiro dicionario.
	string boardFile_path, dictFile_path;
	ifstream file;
	do { // Loop enquanto o ficheiro for inválido
		file.close();
		cout << "Board file name ? "; cin >> boardFile_path; cin.clear(); cin.ignore(10000, '\n');
		file.open(boardFile_path);
	} while (!file.good());
	getline(file, dictFile_path); // tira a primeira linha -> nome do ficheiro do dicionario
	solutionBoard.loadFile(boardFile_path);
	emptyBoard = solutionBoard;
	emptyBoard.grid();
	return dictFile_path;
}

// clues

int getColor(string coords, Board solutionBoard, Board emptyBoard) {
	if (emptyBoard.getWord(coords) == "")
		return WHITE;
	else if (solutionBoard.getWord(coords) == emptyBoard.getWord(coords))
		return LIGHTGREEN;
	else return LIGHTRED;
}

void displayClues(cluemap clues, Board solutionBoard, Board emptyBoard, bool showCorrectGuesses) {
	cout << endl << "HORIZONTAL (H)" << endl;
	for (auto iter : clues) {
		string coords = iter.first;
		if (showCorrectGuesses)
			setcolor(getColor(coords, solutionBoard, emptyBoard)); 
		else if (emptyBoard.getWord(coords) != "") // Se já houver palavra nessas coordenadas!
			setcolor(YELLOW);
		else setcolor(WHITE);
		if (iter.first[2] == 'H') { // iter.first = AaH.
			cout << iter.first.substr(0,2) << ": ";
			for (int i = 0; i < iter.second.size(); i++)
				cout << iter.second[i] << " ";
			size_t wordSize = solutionBoard.getWord(coords).size();
			string wildCard = emptyBoard.getWildcard(coords, wordSize);
			cout << "(" << wordSize << ")";
			if (wildCard.size() == 0) // porque a funçao getWildcard retorna uma string vazia se já estiver lá uma palavra inteira.
				cout << " - " << emptyBoard.getWord(coords) << endl;
			else cout << " - " << emptyBoard.getWildcard(coords, wordSize) << endl;

		}
	}	
	// O mesmo para vertical
	setcolor(WHITE);
	cout << endl << "VERTICAL (V)" << endl;
	for (auto iter : clues) {
		string coords = iter.first;
		if (showCorrectGuesses)
			setcolor(getColor(coords, solutionBoard, emptyBoard));
		else if (emptyBoard.getWord(coords) != "") // Se já houver palavra nessas coordenadas!
			setcolor(YELLOW);
		else setcolor(WHITE);
		if (iter.first[2] == 'V') {
			cout << iter.first.substr(0, 2) << ": ";
			for (int i = 0; i < iter.second.size(); i++)
				cout << iter.second[i] << " ";
			size_t wordSize = solutionBoard.getWord(coords).size();
			string wildCard = emptyBoard.getWildcard(coords, wordSize);
			cout << "(" << wordSize << ")";
			if (wildCard.size() == 0) // porque a funçao getWildcard retorna uma string vazia se já estiver lá uma palavra inteira.
				cout << " - " << emptyBoard.getWord(coords) << endl;
			else cout << " - " << emptyBoard.getWildcard(coords, wordSize) << endl;

		}
	}
	setcolor(WHITE);
	cout << endl;
}

bool addRandomClue(string coords, Board solutionBoard, cluemap &clues, Dictionary dict) {
	string word = solutionBoard.getWord(coords);
	if (word != "") {
		vector<string> givenClues = clues[coords];
		size_t nClues = clues[coords].size();
		size_t nSynonyms = dict.numSynonyms(word);
		if (nClues >= nSynonyms) // Já não há mais sinónimos
			return false;
		string synonym;
		do {
			synonym = dict.getRandomSynonym(word);
		} while (find(givenClues.begin(), givenClues.end(), synonym) != givenClues.end());
		clues[coords].push_back(synonym);
	}
	return false; // coordenadas não válidas. (em principio nunca acontece)
}

cluemap loadRandomClues(Board solutionBoard, Dictionary dict) {
	cluemap clues;
	map<string, string> coordsMap = solutionBoard.getCoordsMap();
	for (auto ite : coordsMap) {
		string coords = ite.first;	string word = ite.second;
		string synonym = dict.getRandomSynonym(word);
		clues[coords].push_back(synonym); 
	}
	return clues;
}

// inserir remover alterar

void delete_at(string coords, Board &b1) {
	string deleted_word = b1.getWord(coords);
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << deleted_word << ".\n";
}

bool replaceable(string coords, string oldword, string newword, Board emptyBoard)
{
	emptyBoard.Delete(coords);
	if (!emptyBoard.Verify(coords, newword))
	{
		emptyBoard.Insert(oldword, coords);
		return false;
	}
	return true;
}

// receber input 

string getInput_Coords(Board solutionBoard) {
	string input_coords;
	flag validCoords;
	do { // Loop enquanto input inválido.
		validCoords = true;
		cout << "Position (LCD) ? ";
		cin >> input_coords; clearBadInput();
		// se forem validas tambem converte o necessario para minusculas e maiusculas!
		// se não houver nenhuma palavra que comece aí as coordenadas são inválidas.
		if (!solutionBoard.validCoords(input_coords) || solutionBoard.getWord(input_coords) == "") 
			validCoords = false;
		if (!validCoords)
			cout << "Invalid Position." << endl;
	} while (!validCoords);
	return input_coords;
}

string getInput_Word(Board emptyBoard, Board solutionBoard, string coords) { //
	string input_word;
	flag validWord;
	do { // Loop enquanto input inválido.
		validWord = false; // palavra inválida até prova em contrário
		cout << "Word ( - = remove / ? = clue / Ctrl + Z = return ) ? ";
		cin >> input_word; 
		if (cin.eof()) {
			cin.clear();
			return "";
		}
		cin.clear(); cin.ignore(10000, '\n');
		stringUpper(input_word);
		string realWord = solutionBoard.getWord(coords); // soluçao
		string currentWord = emptyBoard.getWord(coords); // palavra que está lá.
		if (input_word != "-" && input_word != "?")
		{
			if (realWord.size() != input_word.size()) //verifica se tem o tamanho certo
				cout << "The word " << input_word << " does not have the right size." << endl;
			else if (currentWord != "") // verifica se tem palavra lá
				if (replaceable(coords, currentWord, input_word, emptyBoard)) //verifica se pode substituir
					validWord = true;					
				else cout << "Invalid replacement." << endl;
			else if (!emptyBoard.Verify(coords, input_word)) //ve se coincide com letras anteriores
				cout << "The word " << input_word << " does not fit." << endl;
			else validWord = true;
		}
		else if (input_word == "-" && currentWord == "") //ve se existem palavras para apagar 
			cout << "No words start in that position." << endl;
		else validWord = true;
	} while (!validWord);
	return input_word;
}

string getInput_PlayerName() {
	string name;
	do {
		cin.clear();
		cout << "What's your name? ";
		getline(cin, name);
	} while (cin.eof());
	cout << "Hello " << name << "!\n\n";
	return name;
}

//

void addExtraWords(Board &b1) {
	map<string, string> newWords = b1.extraWords();
	for (auto it = newWords.cbegin(); it != newWords.cend(); it++)
		b1.Insert(it->second, it->first);
}

inline void clearBadInput() {
	if (cin.eof())
		cin.clear();
	else cin.ignore(10000, '\n');
}

void saveStats(Board solutionB1, Dictionary dict, Player p1, size_t timeTaken) {
	ostringstream oss;
	string filePath = solutionB1.saveFile(dict.filePath, true);
	ofstream file(filePath, ios::app);
	file << endl << "Name: " << p1.getName() << " | Clues used: " << p1.getNumClues() << endl;
	file.close();
}

// UI

void displayInstructions() {
	cout << "CROSSWORDS PUZZLE PLAYER" << endl;
	cout << "=========================" << endl;
}

void restartPlayer() {
	char decision;
	cout << endl;
	do { // Loop enquanto input não for "Y" nem "N".
		cout << "Do you want to restart? (Y/N) ";
		cin >> decision; cin.clear(); cin.ignore(10000, '\n');
		decision = toupper(decision); // Para maiúscula
	} while (decision != 'Y' && decision != 'N');
	if (decision == 'N')
		exit(EXIT_SUCCESS);
	else clrscr();
}

bool getInput_showCorrectGuesses() {
	char decision;
	cout << endl;
	do { // Loop enquanto input não for "Y" nem "N".
		cout << "The board is complete but some words still don't match the correct ones. Do you want to know which words you got wrong (Y), or would you prefer to keep trying without help (N) ?";
		cout << "Note: Typing 'Y' will count as a clue. ";
		cin >> decision; clearBadInput();
		decision = toupper(decision); // Para maiúscula
	} while (decision != 'Y' && decision != 'N');
	return decision == 'Y';
}

bool getInput_Difficulty() {
	string decision;
	cout << endl;
	do { // Loop enquanto input não for "EASY" nem "NORMAL".
		cout << "Choose difficulty: " << endl;
		cout << " - Easy" << endl;
		cout << " - Normal" << endl;
		cout << "Option: ";
		cin >> decision; clearBadInput();
		stringUpper(decision); // Para maiúscula
	} while (decision != "EASY" && decision != "NORMAL");
	return decision == "EASY";

}



