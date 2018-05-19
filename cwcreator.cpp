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

typedef bool flag;

char displayInstructions();
void restartCreator();

pair<Board, Dictionary> createPuzzle();
pair<Board, Dictionary> resumePuzzle();

string getInput_Coords(Board b1, Dictionary d1);
string getInput_Word(Board b1, Dictionary d1, string coords);

bool boardBuilding(Board &b1, Dictionary d1);

void delete_at(string coords, Board &b1);
void displaySuggestions(Board b1, Dictionary d1, string coords);
void insert_at(string coords, string word, Board &b1);
bool finalCheck(Board &b1, Dictionary d1);

void ShowVector(vector<string> v);
inline void clearBadInput();

/*
Pergunta ao utilizador o que deseja fazer, e chama as funções de construir o tabuleiro. Quando o tabuleiro for válido, pergunta ao utilizador se deseja recomeçar ou sair.
Board b1 - Board principal
Dictionary d1 - Dictionary principal
pair<Board, Dictionary> Puzzle - Guarda os dois anteriores.
string option - tem de ser 0, 1 ou 2. input do utilizador.
bool boardValid - Board apenas é invalido se estiver terminado e houver palavras inválidas (formadas acidentalmente).
*/
int main() {
	Board b1;
	Dictionary d1;
	pair<Board, Dictionary> Puzzle;
	while (true) { // Faz o ciclo até o utilizador dizer que quer sair.
		char option;
		do { // Faz o ciclo enquanto o utilizador nao der uma opção válida.
			clrscr();
			option = displayInstructions(); // Mostra instruções e retorna a opção do utilizador.
		} while (option != '0' && option != '1' && option != '2');
		if (option == '0')
			exit(0);
		else if (option == '1')
			Puzzle = createPuzzle();
		else if (option == '2')
			Puzzle = resumePuzzle();
		b1 = Puzzle.first; // Puzzle é um par da forma: (board, dictionary)
		d1 = Puzzle.second;
		bool boardValid = false;
		while (!boardValid)
			boardValid = boardBuilding(b1, d1);
		string newBoard_path = b1.saveFile(d1.filePath);
		cout << "Board saved, as " << newBoard_path << ".\n";
		restartCreator(); // Pergunta ao utilizador se quer recomeçar. limpa a consola se for para reiniciar.
	}
}

/*
Enquanto o tabuleiro não estiver cheio: chama as funçoes que pedem as coordenadas e aas palavras.
Quando o utilizador estiver realmente acabado (ou o tabuleiro estiver cheio), enche o tabuleiro e verifica se há palavras extra.
string input_coords - "AaH" por exemplo. "SAVE", "FINISH" são cadeias de retorno especiais, indicam uma ordem ao programa.
string input_word - Será qualquer palavra contida no dicionário que caiba nas coordenadas fornecidas, ou "-" (remove palavra nas coordenadas, se houver), ou "?" (dá sugestões).
*/
bool boardBuilding(Board &b1, Dictionary d1) {
	while (!b1.isFull()) {
		string input_coords = getInput_Coords(b1, d1);
		if (input_coords == "SAVE")
			return true; // ou seja, board válido.
		else if (input_coords == "FINISH") // Utilizador disse que acabou.
			break; // Agora vai verificar se há palavras extra.
		string input_word = getInput_Word(b1, d1, input_coords);
		if (input_word == "-")
			delete_at(input_coords, b1);  // Apaga
		else if (input_word == "?")
			displaySuggestions(b1, d1, input_coords); // Mostra no ecrã todas as palavras que se podem formar.
		else insert_at(input_coords, input_word, b1); // Insere no tabuleiro. Não verifica se cabe nem se existe! (Assume que já foi feito)
		cout << endl;
	}
	if (finalCheck(b1, d1)) { // Se extraWords == true, as palavras novas são todas válidas
		cout << "The board is finished!" << endl;
		b1.Fill();
		return true;
	}
	else return false; // board inválido
}

/*
Mostra as instruções e pergunta ao utilizador o que quer fazer: começar, continuar um puzzle, ou sair.
char option - Opção do utilizador (0, 1, 2)
*/
char displayInstructions() {
	cout << "CROSSWORDS PUZZLE CREATOR" << endl;
	cout << "=========================" << endl;
	cout << "INSTRUCTIONS:" << endl;
	cout << "You will be asked to input a position and its respective word until you don't want to add any extra words to the board." << endl;
	cout << "Position (LCD / Ctrl-Z = stop)" << endl;
	cout << " LCD stands for Line Column and Direction.\n Line and Column will be a letter (limit depends on the size of the board) and Direction will either be 'H' (for Horizontal) or 'V' (for vertical).\n Using Ctrl+Z will finish the board creation." << endl;
	cout << "Word ( - = remove / ? = help)" << endl;
	cout << " If you input an invalid word you will be required to write something else, until your input is valid.\n If you type '-' the word starting from that position will be removed.\n Typing '?' will display every possible insertion." << endl;
	cout << "-------------------------" << endl << endl;
	cout << "OPTIONS:" << endl;
	cout << " 1 - Create puzzle" << endl;
	cout << " 2 - Resume puzzle" << endl;
	cout << " 0 - Exit" << endl << endl;
	cout << "Option ? ";
	char option; cin >> option; clearBadInput();
	return option;
}
/*
Pede o nome do ficheiro do dicionario, abre-o. Pede o tamanho do tabuleiro, cria-o. Devolve os dois.
string dictFile_path - ficheiro de dicionario
int boardSizeX, boardSizeY - colunas e linhas do tabuleiro. Fornecido pelo utilizador.
*/
pair<Board, Dictionary> createPuzzle() {
	string dictFile_path; ifstream file;
	int boardSizeX, boardSizeY;
	cout << "--------------------" << endl;
	cout << "CREATE PUZZLE" << endl;
	cout << "--------------------" << endl;
	do { // Loop enquanto o ficheiro for inválido
		cout << "Dictionary file name ? "; cin >> dictFile_path; clearBadInput();
		file.open(dictFile_path);
	} while (!file.is_open());
	file.close();
	Dictionary d1(dictFile_path);
	do { // Loop enquanto o input for inválido.
		cout << "Board size (lines columns) ? ";
		cin >> boardSizeY >> boardSizeX; clearBadInput(); // SizeY = numero de linhas. SizeX = numero de colunas !
	} while (boardSizeX <= 0 || boardSizeY <= 0 || boardSizeX > Board::MAX_SIZE || boardSizeY > Board::MAX_SIZE);
	cout << endl;
	Board b1(boardSizeX, boardSizeY);
	pair<Board, Dictionary> BoardDict_Pair(b1, d1); // Junta os dois e devolve
	return BoardDict_Pair;
}

/*
Pede o nome do ficheiro do tabuleiro "bxxx.txt", abre-o. Carrega o dicionario e o tabuleiro do ficheiro e devolve os dois.
string boardFile_path - Fornecido pelo utilizador ("bxxx.txt")
string dictFile_path - Fornecido pelo ficheiro de tabuleiro
*/
pair<Board, Dictionary> resumePuzzle() {
	string boardFile_path, dictFile_path;
	ifstream file;
	cout << "--------------------" << endl;
	cout << "RESUME PUZZLE" << endl;
	do { // Loop enquanto o ficheiro for inválido
		file.close();
		cout << "Board file name ? "; cin >> boardFile_path; clearBadInput();
		file.open(boardFile_path);
	} while (!file.good());
	getline(file, dictFile_path); // tira a primeira linha -> nome do ficheiro do dicionario
	Dictionary d1(dictFile_path);
	Board b1; b1.loadFile(boardFile_path);
	pair<Board, Dictionary> BoardDict_Pair(b1, d1); // Junta os dois e devolve
	return BoardDict_Pair;
}

/*
Pede coordenadas na forma LcD ("AaH", por exemplo, Case-Insensitive!). Apenas devolve coordenadas quando forem válidas, isto é, nem podem estar fora do tabuleiro. Se o utilizador fizer Ctrl + Z, pode retornar também "FINISH" se o utilizador quiser terminar, ou "SAVE" se o utilizador quiser guardar para mais tarde.
string input_coords - coordenadas que o utilizador fornece (case insensitive). A retornar.
flag validCoords - flag que mostra se o utilizador já introduziu input válido.
*/
string getInput_Coords(Board b1, Dictionary d1) {
	string input_coords;
	flag validCoords;
	b1.display(); cout << endl;
	do { // Loop enquanto input inválido.
		validCoords = true;
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> input_coords;
		if (cin.eof()) { // Ctrl + Z. Ou se grava para mais tarde ou termina-se
			cin.clear();
			char decision;
			do { // Loop enquanto o utilizador nem diz "S" nem "F". 
				cout << "Save board to resume later (S) or finish creation (F) ? ";
				cin >> decision; clearBadInput();
				decision = toupper(decision);
				if (decision == 'F')
					return "FINISH";
			} while (decision != 'S' && decision != 'F');
			return "SAVE";
		}
		else if (!b1.validCoords(input_coords)) // se forem validas tambem converte o necessario para minusculas e maiusculas!
			validCoords = false;
		if (!validCoords)
			cout << "Invalid Position." << endl;
		cin.ignore(1000, '\n');
	} while (!validCoords);
	return input_coords;
}

/*
Pede uma palavra. Apenas a devolve quando for válida. Isto é, a palavra tem de existir no dicionário, não pode estar no tabuleiro, tem de caber nele. Se o utilizador pedir para remover uma palavra, tem de haver uma palavra nas coordenadas (coords).
string input_word - palavra fornecida (pode ser "-" ou "?" tambem)
flag validWord - indica a validade do input
bool exists - auxiliar. indica se o input existe no dicionario.
*/
string getInput_Word(Board b1, Dictionary d1, string coords) {
	string input_word;
	flag validWord;
	string currentWord = b1.getWord(coords);
	do { // Loop enquanto input inválido.
		validWord = true;
		cout << "Word ( - = remove / ? = help ) ? ";
		cin >> input_word;
		if (cin.eof())
			cin.clear();
		else cin.ignore(10000, '\n');
		stringUpper(input_word);
		bool exists = d1.wordExists(input_word);
		if (!exists && input_word != "-" && input_word != "?") { // Palavra não existe.
			cout << "The word " << input_word << " does not exist in the dictionary!" << endl;
			validWord = false;
		}
		else if (exists && b1.hasWord(input_word)) { // Palavra existe, mas board já a tem.
			cout << "The word " << input_word << " is already in the board!" << endl;
			validWord = false;
		}
		else if (exists && currentWord != "") //Posicao do board ja ocupada
		{
			if (!b1.replaceable(coords, currentWord, input_word))  //nao pode ser substituida
			{
				cout << "Invalid Replacement" << endl;
				validWord = false;
			}
		}
		else if (exists && !b1.Verify(coords, input_word)) { // Palavra existe, mas não pode ser colocada.
			cout << "The word " << input_word << " does not fit in the board!" << endl;
			validWord = false;
		}
		else if (input_word == "-" && b1.getWord(coords) == "") { // Quer-se remover palavra, mas não há nenhuma nessas coordenadas.
			cout << "No words start in that position." << endl;
			validWord = false;
		}
	} while (!validWord);
	return input_word;
}

/*
Pergunta ao utilizador se quer recomeçar o programa. Se a resposta for "N", fecha o programa. Se for "Y", não faz nada.
char decision - input.
*/
void restartCreator() {
	char decision;
	cout << endl;
	do { // Loop enquanto input não for "Y" nem "N".
		cout << "Do you want to restart? (Y/N) ";
		cin >> decision; clearBadInput();
		decision = toupper(decision); // Para maiúscula
	} while (decision != 'Y' && decision != 'N');
	if (decision == 'N')
		exit(EXIT_SUCCESS);
	else clrscr();
}

/*
Tenta apagar a palavra que começa em coords (formato LcD).
string deleted_word - palavra que foi apagada.
*/
void delete_at(string coords, Board &b1) {
	string deleted_word = b1.getWord(coords);
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << deleted_word << ".\n";
}

/*
Mostra no ecrã todas as palavras que se podem fazer no board a partir de coords (LcD). Recebe um dicionário tambem. O que a função faz é começa na posição desejada e faz uma wildcard desde a posição até N casas à frente (por exemplo "P???R"). Faz isso para todos os tamanhos possíveis desde 1 até ao máximo que o tabuleiro aguenta nessa direção. Para cada wilcard é usada a função getwildcardmatches do trabalho 1.
vector<string> sugestoes - vetor a retornar. conterá todas as sugestoes
Cursor c1 - cursor auxiliar.
string wildcard - Exemplo, "P???R".
vector<string> wildcards - todas as possibilidades para wilcard
*/
void displaySuggestions(Board b1, Dictionary d1, string coords) {
	vector<string> sugestoes;
	Cursor c1; c1.moveTo(coords); // move o cursor até as coordenadas.
	int wordSize = 1; // wordSize vai de 1 até ao limite do tabuleiro.
	cout << "Fecthing suggested words..." << endl;
	for (int i = c1.MainCoord(); i < b1.CoordLimit(c1); i++) { // c1.MainCoord() = coordenada na direção em que está. CoordLimit(c1) = coordenada máxima do board na direção de c1.
		string wildcard = b1.getWildcard(coords, wordSize); // Recebe a wilcard a partir de coords, tamanho wordSize.
		vector<string> wildcards = d1.getWildcardMatches(wildcard); // recebe as possiveis palavras associadas ao wordSize atual, com as coordenadas fornecidas.
		for (int i = 0; i < wildcards.size(); i++) { // Insere essas palavras no vetor, se não estiverem no board!
			if (!b1.hasWord(wildcards[i]))
				sugestoes.push_back(wildcards[i]);
		} 
		wordSize++;
	}
	if (sugestoes.empty())
		cout << "No words found in the dictionary." << endl;
	else {
		cout << "Suggested Words: ";
		ShowVector(sugestoes);
	}
}


void insert_at(string coords, string word, Board &b1) { // Insere...
	b1.Insert(word, coords);
	cout << "The word " << word << " has been inserted." << endl;
}

void ShowVector(vector<string> v) // Mostra os conteúdos de um vetor de strings
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " ";
	cout << endl;
}

inline void clearBadInput() {
	if (cin.eof())
		cin.clear();
	else {
		cin.clear();
		cin.ignore(10000, '\n');
	}
}

/*

*/
bool finalCheck(Board &b1, Dictionary d1)
{
	map<string, string> newWords = b1.extraWords();
	map<string, string> validWords, invalidWords;
	cout << endl;
	for (auto it = newWords.cbegin(); it != newWords.cend(); it++)
	{
		if (d1.wordExists(it->second) && !b1.hasWord(it->second))	//separa as palavras validas das invalidas em dois maps diferentes
			validWords[it->second] = it->first;
		else invalidWords[it->second] = it->first;
	}
	if (invalidWords.size() > 0) {	//mostra as palavras novas invalidas
		if (invalidWords.size() == 1) cout << "The board still has an invalid word:\n";
		else cout << "The board still has some invalid words:\n";
		for (auto it = invalidWords.cbegin(); it != invalidWords.cend(); it++)
			cout << " " << it->second << " " << it->first << "\n";
		cout << endl << "You cannot finish creation until those words get removed." << endl;
		return false;  //se existirem palavra invalidas
	}
	else if (validWords.size() > 0) { //mostra as palavras novas validas
		if (validWords.size() == 1) cout << "This word was automatically inserted:\n";
		else cout << "These words were automatically inserted:\n";
		for (auto it = validWords.cbegin(); it != validWords.cend(); it++)
			cout << it->second << " " << it->first << "\n";
	}
	cout << endl;
	for (auto it = newWords.cbegin(); it != newWords.cend(); it++)
		b1.Insert(it->second, it->first);
	return true;
}

