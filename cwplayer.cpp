#include "stdafx.h"
#include "dictionary.h"
#include "consolecolors.h"
#include "board.h"
#include "Player.h"
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

// Funçoes para as clues
int getDisplayColor(string coords, Board solutionBoard, Board emptyBoard);
void displayClues(cluemap clues, Board solutionBoard, Board emptyBoard, bool showCorrectGuesses);
bool addRandomClue(string coords, Board solutionBoard, cluemap &clues, Dictionary dict);
cluemap loadRandomClues(Board solutionBoard, Dictionary dict);

// Apagar palavras
void delete_at(string coords, Board &b1);

// Recebe input para nome coordenadas e palavra
string getInput_PlayerName();
string getInput_Coords(Board solutionBoard);
string getInput_Word(Board emptyBoard, Board solutionBoard, string coords);

// Outras funções
string openBoard(Board &solutionBoard, Board &emptyBoard);
void addValidExtraWords(Board &b1, Board solutionBoard);

inline void clearBadInput();
void saveStats(Board solutionB1, Player p1);

// UI / Opções
void displayInstructions();
void restartPlayer();
bool getInput_showCorrectGuesses();
bool getInput_Difficulty();

/*
Carrega um Board, copia-o para outro e esvazia as palavras. Carrega o dicionário. Cria um jogador. Carrega um sinónimo para cada palavra. Enquanto o tabuleiro não estiver resolvido, pede ao utilizador coordenadas e uma palavra (ou se quer uma pista ou se quer remover), coloca-a no sítio. Quando o tabuleiro estiver cheio verifica se está correto. Se sim, guarda o tempo gasto e grava a informação do jogador num ficheiro.

Board solutionB1 - board com a solução.
Board b1 - board a ser resolvido
cluemap clues - map<string, vector<string>> com as coordenadas das respostas e alguns sinónimos (clues)
flag boardSolved - indica se o tabuleiro já foi resolvido
bool easyMode - indica dificuldade
bool showCorrectGuesses - True: mostram-se a verde as respostas certas e vermelho as erradas
*/
int main() {
	srand((unsigned int)time(NULL));
	while (true) {
		Board solutionB1, b1; // b1 = board a ser resolvido.
		Dictionary d1;
		Player p1;
		displayInstructions();
		p1.setName(getInput_PlayerName()); // Pede o nome do utilizador
		string dictFile_path = openBoard(solutionB1, b1); // open board abre solutionB1 e b1.
		d1.load(dictFile_path);
		cluemap clues = loadRandomClues(solutionB1, d1);
		flag boardSolved = false;
		bool easyMode = getInput_Difficulty();
		if (easyMode) p1.setEasyMode();
		bool showCorrectGuesses = false;
		cout << "Clock starts now!" << endl;
		p1.startClock();
		do {
			b1.display();
			displayClues(clues, solutionB1, b1, showCorrectGuesses || easyMode);
			cout << "Clues used so far: " << p1.getNumClues() << endl << endl;
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
			else {
				b1.Insert(word, coords);
				addValidExtraWords(b1, solutionB1);
			}
			// BOARD CHEIO!
			if (b1.isFull()) {
				b1.display();
				displayClues(clues, solutionB1, b1, solutionB1 == b1); // se solution == b1, showCorrectGuesses = true.
				if (solutionB1 == b1) // == compara os boards!
					boardSolved = true;
				else if (easyMode)
					continue;
				else if (getInput_showCorrectGuesses()) {
					p1.incClues();
					showCorrectGuesses = true;
				}
			}

		} while (!boardSolved);
		p1.endClock();
		cout << "Congratulations! You completed the board in " << p1.getTimeTaken() << " seconds." << endl;
		saveStats(solutionB1, p1);
		restartPlayer(); // Pergunta ao utilizador se quer recomeçar. limpa a consola se for para reiniciar.
	}
}



// Recebe as coordenadas, a solução e a tentativa do utilizador. Retorna verde se a tentativa for correta, vermelha se for errada. Branco se não estiver nenhuma palavra
int getDisplayColor(string coords, Board solutionBoard, Board emptyBoard) {
	if (emptyBoard.getWord(coords) == "") // Se não houver palavra nas cordenadas.
		return WHITE;
	else if (solutionBoard.getWord(coords) == emptyBoard.getWord(coords)) // Se solução for correta.
		return LIGHTGREEN;
	else return LIGHTRED;
}

/*
Mostra as coordenadas e as pistas que há, primeiro as Horizontais e depois as Verticais. Se showCorrectGuesses == true, mostra as respostas certas a verde e as erradas a vermelho. Se não, mostra as preenchidas a amarelo. As não preenchidas ficam sempre a branco. Mostra tambem o tamanho das respostas e as letras que já estão no tabuleiro (restrições).

int numWordsHoriz = 0, numWordsVertical = 0 - São apenas contadores das palavras horizontais e verticais.
size_t wordSize - tamanho da palavra correta
string wildCard - para essas coordenadas: pontos de interrogação/letras dependendo se a casa está vazia ou não. (ex: "I??A?")
*/
void displayClues(cluemap clues, Board solutionBoard, Board emptyBoard, bool showCorrectGuesses) {
	cout << endl << "HORIZONTAL (H)" << endl;
	int numWordsHoriz = 0, numWordsVertical = 0;
	for (auto iter : clues) { // Percorre clues
		string coords = iter.first; // coordenadas para esta iteração
		vector<string> synonyms = iter.second; // sinónimos para estas coordenadas
		if (showCorrectGuesses)
			setcolor(getDisplayColor(coords, solutionBoard, emptyBoard));
		else if (emptyBoard.getWord(coords) != "") // Se já houver palavra nessas coordenadas!
			setcolor(YELLOW);
		else setcolor(WHITE);
		if (coords[2] == 'H') {  // Se forem coordenadas com direçao vertical
			cout << coords.substr(0, 2) << ": ";
			for (size_t i = 0; i < synonyms.size(); i++)
				cout << synonyms[i] << " ";
			size_t wordSize = solutionBoard.getWord(coords).size();
			string wildCard = emptyBoard.getWildcard(coords, wordSize);
			cout << "(" << wordSize << ")";
			cout << " - " << wildCard << endl;
			numWordsHoriz++;
		}
	}
	setcolor(WHITE);
	if (numWordsHoriz == 0)
		cout << "No horizontal words." << endl;
	// O mesmo para vertical
	cout << endl << "VERTICAL (V)" << endl;
	for (auto iter : clues) {  // Percorre clues
		string coords = iter.first; // coordenadas para esta iteração
		vector<string> synonyms = iter.second; // sinónimos para estas coordenadas
		if (showCorrectGuesses)
			setcolor(getDisplayColor(coords, solutionBoard, emptyBoard));
		else if (emptyBoard.getWord(coords) != "") // Se já houver palavra nessas coordenadas!
			setcolor(YELLOW);
		else setcolor(WHITE);
		if (coords[2] == 'V') { // Se forem coordenadas com direçao vertical
			cout << coords.substr(0, 2) << ": ";
			for (size_t i = 0; i < synonyms.size(); i++)
				cout << synonyms[i] << " ";
			size_t wordSize = solutionBoard.getWord(coords).size();
			string wildCard = emptyBoard.getWildcard(coords, wordSize);
			cout << "(" << wordSize << ")";
			cout << " - " << wildCard << endl;
			numWordsVertical++;
		}
	}
	setcolor(WHITE);
	if (numWordsVertical == 0)
		cout << "No vertical words." << endl;
	cout << endl;
}

/*
Obtem a palavra correta, obtem as pistas já fornecidas e o número de sinónimos que há para a palavra correta. Verifica que ainda há sinónimos suficientes, vai obtendo um sinónimo à sorte até que se obtenha um que não se tenha ainda fornecido.

string answer - resposta correta
vector<string> givenClues - pistas já fornecidas
size_t nClues - número de pistas já fornecidas para a palavra
size_t nSynonyms - número de sinónimos existentes para a palavra
string synonym - sinónimo que se vai acrescentar
*/
bool addRandomClue(string coords, Board solutionBoard, cluemap &clues, Dictionary dict) {
	string answer = solutionBoard.getWord(coords);
	if (answer != "") {
		vector<string> givenClues = clues[coords];
		size_t nClues = clues[coords].size();
		size_t nSynonyms = dict.numSynonyms(answer);
		if (nClues >= nSynonyms) // Já não há mais sinónimos
			return false;
		string synonym;
		do {
			synonym = dict.getRandomSynonym(answer);
		} while (find(givenClues.begin(), givenClues.end(), synonym) != givenClues.end());
		clues[coords].push_back(synonym);
		return true;
	}
	return false; // coordenadas não válidas. (em principio nunca acontece)
}

/*
Vai buscar o mapa de coordenadas das solução. Por cada palavra lá obtém-se um sinónimo.

cluemap clues - mapa de pistas a devolver (map<string, vector<string>>)
string coords - coordenadas
string word - palavra
string synonym - sinónimo aleatório a acrescentar ao vetor correspondente a coords em clues.
*/
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

// Apaga a palavra em coords de b1.
void delete_at(string coords, Board &b1) {
	string deleted_word = b1.getWord(coords);
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << deleted_word << ".\n";
}

// Recebe nome do jogador
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

// Recebe coordenadas
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

/*
Recebe uma palavra para colocar nas coordenadas (coords), ou indicação para remover  uma palavra, ou acrescentar uma pista. Verifica se a palavra tem o tamanho certo, se pode subtituir caso esteja lá uma já, se cabe no tabuleiro, ou se existem palavras para apagar (no caso de se responder "-").

flag validWord - indica se input válido
string realWord - soluçao para essas coords
string currentWord - palavra que está no tabuleiro que o utilizador preenche
*/
string getInput_Word(Board emptyBoard, Board solutionBoard, string coords) { //
	string input_word;
	flag validWord;
	do { // Loop enquanto input inválido.
		validWord = false; // palavra inválida até prova em contrário
		cout << "Word ( - = remove / ? = clue / CTRL-Z = return ) ? ";
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
				if (emptyBoard.replaceable(coords, currentWord, input_word)) //verifica se pode substituir
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

/*
Pede o nome de um ficheiro de tabuleiro. Carrega solutionBoard, copia-o para emptyBoard (board a ser resolvivo). Emptyboard perde todas as palavras, ficando apenas com as células pretas. Retorna o nome do ficheiro dicionario.

string boardFile_path, dictFile_path - nome dos ficheiros
ifstream file - ficheiro Board
*/
string openBoard(Board &solutionBoard, Board &emptyBoard) {
	string boardFile_path, dictFile_path;
	ifstream file;
	do { // Loop enquanto o ficheiro for inválido
		file.close();
		cout << "Board file name ? "; cin >> boardFile_path; clearBadInput();
		file.open(boardFile_path);
	} while (!file.good());
	getline(file, dictFile_path); // tira a primeira linha -> nome do ficheiro do dicionario
	solutionBoard.loadFile(boardFile_path);
	emptyBoard = solutionBoard;
	emptyBoard.grid(); // Ficam apenas as células pretas (perde as palavras)
	return dictFile_path;
}

/*
Recebe um tabuleiro que está a ser preenchido pelo utilizador e a solução. Vê se alguma palavra que foi introduzida acidentalmente pelo utilizador realmente corresponde a uma entrada no tabuleiro solução, ou seja, se começa nalguma casa válida e se tem o tamanho correto. Adiciona todas essas palavras novas válidas. (Mesmo que já estejam no tabuleiro visual, o programa ainda não contava estas palavras)

map<string, string> newWords - todas as palavras que estão no tabuleiro mas não estão no map de palavras
*/
void addValidExtraWords(Board &b1, Board solutionBoard) {
	map<string, string> newWords = b1.extraWords();
	for (auto it = newWords.cbegin(); it != newWords.cend(); it++) { // percorre o mapa de palavras novas
		string coords = it->first;
		string word = it->second;
		if (solutionBoard.getWord(coords).size() == word.size()) // se a palavra que está no tabuleiro com essas coordenadas tem tamanho igual à palavra nova, é válida.
			b1.Insert(word, coords); // insere no tabuleiro 
	}
}

//Função auxiliar, para melhorar experiência do utilizador. Se input for Ctrl-Z, limpa-se o buffer. Se não, ignora-se toda a linha.
inline void clearBadInput() {
	if (cin.eof())
		cin.clear();
	else {
		cin.clear();
		cin.ignore(10000, '\n');
	}
}

/*
Recebe a solução, o dicionário, o jogador e o tempo gasto. Guarda num ficheiro "bxxx_p.txt" o tabuleiro (se o ficheiro não existir) e acrescenta no fim uma linha mostrando o Nome do jogador, o Tempo que ele demorou a resolver e o número de pistas usadas. Se estiver a jogar em modo fácil acrescenta também essa informação.

size_t boardNum - número do tabuleiro
ostringstream oss - guarda "bxxx_p.txt"
string file_path = oss.str() - guarda o ficheiro onde se irá guardar
*/
void saveStats(Board solutionB1, Player p1) {
	size_t boardNum = solutionB1.getBoardNumber(); // obtém o número do tabuleiro
	ostringstream oss;
	oss << "b" << setfill('0') << setw(3) << boardNum; oss << "_p.txt"; // oss = bxxx_p.txt
	string file_path = oss.str();
	ofstream file(file_path, ios::app); // abre o ficheiro no final, depois acrescenta a informação.
	file << "Name: " << p1.getName() << " | Time taken: " << p1.getTimeTaken() << " seconds | Clues used: " << p1.getNumClues();
	if (!p1.isNormalMode())
		file << " | EASY MODE";
	file << endl;
	file.close();
}

// Mostra instruções
void displayInstructions() {
	setcolor(WHITE);
	cout << "CROSSWORDS PUZZLE PLAYER" << endl;
	cout << "=========================" << endl;
	cout << "INSTRUCTIONS:" << endl;
	cout << "You will be asked to input a position and its respective word until the board is solved." << endl << endl;
	cout << "Position (LCD)" << endl;
	cout << " - LCD stands for Line Column and Direction." << endl;
	cout << " - Line and Column will be a letter each, and Direction will either be 'H' (for Horizontal) or 'V' (for vertical). Input is case-insensitive." << endl;
	cout << " - You will only be able to insert positions which correspond to the beggining of a solution" << endl << endl;
	cout << "Word ( - = remove / ? = clue / CTRL-Z = return )" << endl;
	cout << " - You will be required to input any word that fits in the board, but it needs to respect the letters from the previously placed words." << endl;
	cout << " - If you type '-' the word starting from that position will be removed." << endl;
	cout << " - Typing '?' will add an extra synonym to the clues for that word." << endl;
	cout << " - CTRL-Z will return to the position selection." << endl;
	cout << " - You are allowed to replace the word on the position you picked." << endl;
	cout << "-------------------------" << endl;
	cout << "CLUES: The synonyms will be shown in a specific way and colors:" << endl;
	setcolor(WHITE);
	cout << "  Word Synonym (SIZE) - EmptySpots   (EMPTY) " << endl;
	setcolor(YELLOW);
	cout << "  Word Synonym (SIZE) - EmptySpots   (FILLED)" << endl;
	setcolor(LIGHTGREEN);
	cout << "  Word Synonym (SIZE) - EmptySpots   (CORRECT)" << endl;
	setcolor(LIGHTRED);
	cout << "  Word Synonym (SIZE) - EmptySpots   (WRONG) " << endl;
	setcolor(WHITE);
	cout << "Note: Green/Red colors will only show up during specific occasions." << endl;
	cout << "-------------------------" << endl << endl;
}

// Pergunta ao utilizador se deseja recomeçar. Se sim, limpa a consola. Se não, fecha o programa.
void restartPlayer() {
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

// Pergunta ao utilizador se deseja mostrar quais respostas estão mal. Fazê-lo gasta uma pista. (Chamada quando o tabuleiro está completo com palavras mal) 
bool getInput_showCorrectGuesses() {
	char decision;
	do { // Loop enquanto input não for "Y" nem "N".
		cout << "The board is complete but some words still don't match the correct ones. Do you want to know which words you got wrong (Y), or would you prefer to keep trying without help (N) ? ";
		cout << "Note: Typing 'Y' will count as a clue. ";
		cin >> decision; clearBadInput();
		decision = toupper(decision); // Para maiúscula
	} while (decision != 'Y' && decision != 'N');
	return decision == 'Y';
}

// Pergunta ao utilizador a dificuldade com que quer jogar. Só pode ser easy ou normal (case-insensitive).
bool getInput_Difficulty() {
	string decision;
	cout << endl;
	do { // Loop enquanto input não for "EASY" nem "NORMAL".
		cout << "Choose difficulty: " << endl;
		cout << " - Easy   (Always show correct guesses)" << endl;
		cout << " - Normal (No changes)" << endl;
		cout << "Option: ";
		cin >> decision; clearBadInput();
		stringUpper(decision); // Para maiúscula
		cout << endl;
	} while (decision != "EASY" && decision != "NORMAL");
	return decision == "EASY";
}
