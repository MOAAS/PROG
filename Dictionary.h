#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

class Dictionary {
public:
	Dictionary();
	Dictionary(string textFile); // Carrega um dicionário
	void load(string textFile);  // Carrega um dicionário
	bool wordExists(string word) const; // Verifica se uma palavra existe
	vector<string> getWildcardMatches(string wildcardWord) const; // Obtém todas as possibilidades de uma wildcard com letras e '?'
	string getRandomSynonym(string word) const; // Obtém um sinónimo aleatório de uma palavra
	size_t numSynonyms(string word); // Obtém o número de sinónimos para uma palavra
	string filePath; // nome do ficheiro de texto onde está o dicionário
private:
	map<string, vector<string>> words; // Contem para cada palavra um vetor de sinónimos
};

void stringUpper(string &input); // Converte uma string para letras maiúsculas
bool wildcardMatch(const char *str, const char *strWild); // Verifica se uma palavra corresponde a uma wildcardword
bool lineValid(string &line); // Verifica se uma certa linha do ficheiro de texto é válida. Também remove ':' e ';' de line. 
