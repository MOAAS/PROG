#include "stdafx.h"
#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;


Dictionary::Dictionary() {

}

Dictionary::Dictionary(string textFile) { 
	load(textFile);
}


bool Dictionary::wordExists(string word) const{
	stringUpper(word);
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		if (ite->first == word)
			return true;
	}
	return false;
}

// Transfere as palavras e os sinonimos do ficheiro de texto para o dicionario
void Dictionary::load(string filePath) {
	string line, keyWord, synonym; // Variaveis auxiliares.
	ifstream file(filePath);
	if (file.is_open()) { 
		while (getline(file, line)) { // Retira uma linha do ficheiro, guarda em line.
			if (!lineValid(line))
				continue;
			istringstream iss(line); // Coloca line em iss.
			iss >> keyWord; stringUpper(keyWord);	// Inicializa keyWord, apaga todos os carateres a mais. Coloca todos os carateres em maiusculas.
			while (iss >> synonym) { // Faz o mesmo com todos os sinonimos.
				stringUpper(synonym);
				words[keyWord].push_back(synonym); // Coloca no vetor associado a keyWord.
			}
		}
		this->filePath = filePath;
	}
	else { // Se o ficheiro nao abrir
		cout << "File could not be opened." << endl;
	}

}

// Mostra as palavras do dicionario e os seus sinonimos. debug
void Dictionary::display() const {
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		cout << ite->first << ": ";
		for (int i = 0; i < (ite->second).size(); i++)
			cout << (ite->second)[i] << ", ";
		cout << endl;
	}
}

// Recebe uma string com wildcards (wildcardWord). Devolve um vetor com todas as strings que podem ser formadas.

vector<string> Dictionary::getWildcardMatches(string wildcardWord) const {
	vector<string> matches;
	for (auto it : words) {
		string key = it.first;
		if (wildcardMatch(key.c_str(), wildcardWord.c_str())) {
			matches.push_back(it.first);
		}
	}
	return matches;
}
// Se a palavra não existir, devolve uma string vazia.
string Dictionary::getRandomSynonym(string word) const {
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		if (ite->first == word && (ite->second).size() != 0) { // A segunda condição apenas está lá por razões de segurança, pois deverá sempre haver sinónimos.
			vector<string> synonyms = ite->second;
			size_t rand_index = rand() % synonyms.size();
			return synonyms[rand_index];
		}
	}
	return "";
}

bool lineValid(string &line) { // Remove ':' e ';' de line. Retorna falso se a linha não for válida.
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ':' || line[i] == ',') {
			line.erase(i, 1);
		}
		if (!isalpha(line[i]) && line[i] != ' ')
			return false;
	}
	return true;
}

// Programa fornecido pelo professor. Verifica se uma string com wildcards pode corresponder a outra.

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
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

void stringUpper(string &input) { // Converte uma string para upper case. Passada por referencia.
	transform(input.begin(), input.end(), input.begin(), ::toupper);
}
