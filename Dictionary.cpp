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

// Percorre o map inteiro. Devolve true se a palavra existir (no lado esquerdo do map)
bool Dictionary::wordExists(string word) const{
	stringUpper(word);
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		if (ite->first == word)
			return true;
	}
	return false;
}

/* 
Transfere as palavras e os sinonimos do ficheiro de texto para o map. Para cada linha do ficheiro, pega na primeira palavra e guarda-a. Todas as palavras seguintes contam como sinónimos. Guarda no map os sinónimos para essa palavra. 

string line - linha do ficheiro de texto (variavel temporaria)
string keyWord - palavra (lado esquerdo do map)
string synonym - um dos sinónimos de keyWord
ifstream file - ficheiro do dicionario
istringstream iss - contem cada linha do ficheiro de texto, auxilia a inserção de palavras.

*/
void Dictionary::load(string filePath) {
	string line, keyWord, synonym; // Variaveis auxiliares.
	ifstream file(filePath);
	if (file.is_open()) { 
		cout << "Loading dictionary... ";
		while (getline(file, line)) { // Retira uma linha do ficheiro, guarda em line.
			if (!lineValid(line))
				continue;
			istringstream iss(line); // Coloca line em iss.
			iss >> keyWord; stringUpper(keyWord);	// Inicializa keyWord, apaga todos os carateres a mais. Coloca todos os carateres em maiusculas.
			while (iss >> synonym) { // Faz o mesmo com todos os sinonimos ^
				stringUpper(synonym);
				words[keyWord].push_back(synonym); // Coloca no vetor associado a keyWord.
			}
		}
		cout << "Done." << endl;
		this->filePath = filePath;
	}
	else { // Se o ficheiro nao abrir
		cout << "File could not be opened." << endl;
	}

}


/* 
Recebe uma string com wildcards (wildcardWord). Devolve um vetor com todas as strings que podem ser formadas, percorrendo todo o map de palavras e utilizando a função auxiliar wildcard match para cada palavra.
string key - palavra (lado esquerdo do map)
*/
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

// Encontra os sinónimos para a palavra fornecida, percorrendo o map todo. Escolhe um índice aleatório e devolve o sinónimo.
string Dictionary::getRandomSynonym(string word) const {
	for (map<string, vector<string>>::const_iterator ite = words.begin(); ite != words.end(); ite++) {
		if (ite->first == word && (ite->second).size() != 0) { // A segunda condição apenas está lá por razões de segurança, pois deverá sempre haver sinónimos.
			vector<string> synonyms = ite->second;
			size_t rand_index = rand() % synonyms.size();
			return synonyms[rand_index];
		}
	}
	return ""; // Se a palavra não existir, devolve uma string vazia.

}

size_t Dictionary::numSynonyms(string word)
{
	return words[word].size();
}


bool lineValid(string &line) { 
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ':' || line[i] == ',') { // Ao encontrar um desses dois, apaga-se.
			line.erase(i, 1);
		}
		if (!isalpha(line[i]) && line[i] != ' ') // Se o carater nao for alfabético nem um espaço, retorna-se falso.
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
	return !*str && !*strWild;
}

void stringUpper(string &input) {
	transform(input.begin(), input.end(), input.begin(), ::toupper);
}
