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

void delete_at(string coords, Board &b1);
bool verify_insert_replace(string coords, string word, Board &b);
string getInput_Coords(Board b1);
string getInput_Word(Board bcopia, Board b, string coords) ;
bool replacable(string coords, string oldword, string newword, Board b);

typedef bool flag;

int main() //ultra mega beta test
{

	Board b1, bcopia;
	b1.loadFile("b001.txt");
	bcopia = b1;
	bcopia.grid();
	bcopia.display();
	string word, coords ,realword;
	while (1)
	{
		coords = getInput_Coords(b1);
		word = getInput_Word(bcopia, b1, coords);
		if (word == "-")
			delete_at(coords, bcopia);
		else  bcopia.Insert(word, coords);
		bcopia.display();
	}
}

void delete_at(string coords, Board &b1) {
	string deleted_word = b1.getWord(coords);
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << deleted_word << ".\n";
}


string getInput_Coords(Board b1) {
	string input_coords;
	flag validCoords;
	do { // Loop enquanto input inválido.
		validCoords = true;
		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> input_coords;
		if (!b1.validCoords(input_coords) || b1.getWord(input_coords)=="") // se forem validas tambem converte o necessario para minusculas e maiusculas!
			validCoords = false;
		if (!validCoords)
			cout << "Invalid Position." << endl;
		cin.ignore(1000, '\n');
	} while (!validCoords);
	return input_coords;
}

string getInput_Word(Board bcopia, Board b , string coords) {
	string input_word;
	flag validWord;
	do { // Loop enquanto input inválido.
		validWord = true;
		cout << "Word ( - = remove / ? = help ) ? ";
		cin >> input_word; cin.clear(); cin.ignore(10000, '\n');
		stringUpper(input_word);
		string realword = b.getWord(coords);
		string replaceWord = bcopia.getWord(coords);
		bool isWord = (input_word != "-");

		if (isWord)
		{
			if (realword.size() != input_word.size()) //verifica se tem o tamanho certo
			{
				cout << "The word " << input_word << " does not have the right size" << endl;
				validWord = false;
			}
			else if (replaceWord != "")   //verifica ja existe algo antes 
			{
				if (!replacable(coords, replaceWord , input_word, bcopia)) //verifica se pode mudar
				{
				cout << "Invalid replacement" << endl;
				validWord = false;
				}
			}
			else if (!bcopia.Verify(coords, input_word)) //ve se coincide com letras anteriores
			{
				cout << "The word " << input_word << " does not fit" << endl;
				validWord = false;
			}
		}
		else
			if (replaceWord == "")  //ve se existem palavras para apagar
				cout << "No words start in that position." << endl; //se nao existir , continua para nao ficar preso

	} while (!validWord);
	return input_word;
}

bool replacable(string coords, string oldword, string newword,Board b)
{
	b.Delete(coords);
	if (!b.Verify(coords, newword))
	{
		b.Insert(oldword, coords);
		return false;
	}
	return true;
}
