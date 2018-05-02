#include "stdafx.h"
#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <sstream>


Dictionary::Dictionary() {

}

bool Dictionary::wordExists(string word) const{
	stringUpper(word);
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		if (ite->first == word)
			return true;
	}
	return false;
}

void Dictionary::load(string filePath) {
	string line, keyWord, synonym; // Variaveis auxiliares.
	ifstream file(filePath);
	if (file.is_open()) { 
		while (getline(file, line)) { // Retira uma linha do ficheiro, guarda em line.
			istringstream iss(line); // Coloca line em iss.
			iss >> keyWord; alphaOnly(keyWord); stringUpper(keyWord);	// Inicializa keyWord, apaga todos os carateres a mais. Coloca todos os carateres em maiusculas.
			while (iss >> synonym) { // Faz o mesmo com todos os sinonimos.
				alphaOnly(synonym); stringUpper(synonym);
				words[keyWord].push_back(synonym); // Coloca no vetor associado a keyWord.
			}
		}
	}
	else { // Se o ficheiro nao abrir
		cout << "File could not be opened." << endl;
	}
}

void Dictionary::display() const {
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++)
	{
		cout << ite->first << ": ";
		for (int i = 0; i < (ite->second).size(); i++)
			cout << (ite->second)[i] << ", ";
		cout << endl;

	}
	for (auto ite:words)
	{
		cout << ite.first << ": ";
		for (int i = 0; i < (ite.second).size(); i++)
			cout << (ite.second)[i] << ", ";
		cout << endl;

	}
}

vector<string> Dictionary::getWildcardMatches(string wildcardWord) const {
	vector<string> matches;
	for (auto it : words) {
		string key = it.first;
		if (wildcardMatch(key.c_str, wildcardWord.c_str())) {
			matches.push_back(it.first);
		}
	}
	return matches;
}

bool wildcardMatch(const char *str, const char *strWild) {
	while (*strWild) {
		if (*strWild == '?') {
			if (!*str)
				return false;
			++str;
			++strWild;
		}
		else if (*strWild == '*') {
			if (wildcardMatch(str, strWild + 1))
				return true;
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			return false;
		}
		else {
				return false;
		}
	}
	return !*str && !*strWild;
}


void stringUpper(string &input) { // Converte uma string para upper case. Passada por referencia.
	transform(input.begin(), input.end(), input.begin(), ::toupper);
}

void alphaOnly(string &str) { // Apaga qualquer carater nao alfanumerico
	for (int i = 0; i < str.size(); i++) {
		if (!(isalpha(str[i])))
			str.erase(i, 1);
	}
}