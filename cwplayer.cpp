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
		cin >> coords >> word;
		realword = b1.getWord(coords);
		if (word == "-")
			delete_at(coords, bcopia);
		else if (realword == "")
			cout << "COORDENADAS PODRES" << endl;
		else if (realword.size() != word.size())
			cout << "TAMANHO PODRE" << endl;
		else if (!verify_insert_replace(coords, word ,bcopia))
			cout << "NAO CABE" << endl;
		bcopia.display();
	}
}

void delete_at(string coords, Board &b1) {
	string deleted_word = b1.getWord(coords);
	if (b1.Delete(coords)) // Tenta apagar
		cout << "Successfully deleted the word " << deleted_word << ".\n";
}

bool verify_insert_replace(string coords,string word, Board &b)
{
	string replaceWord;
	replaceWord = b.getWord(coords);
	if (replaceWord == "")
	{
		if (b.Verify(coords, word))
		{
			b.Insert(word, coords);
			return true;
		}
		else return false;
	}
	else //replace
	{
		b.Delete(coords);
		if (b.Verify(coords, word))
		{
			b.Insert(word, coords);
			return true;
		}
		else
		{
			b.Insert(replaceWord, coords);
			return false;
		}
	}
}
