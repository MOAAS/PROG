#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

class Dictionary {
public:
	Dictionary();
	Dictionary(string textFile); 
	void load(string textFile);
	bool wordExists(string word) const;
	void display() const;
	vector<string> getWildcardMatches(string wildcardWord) const;
	string getRandomSynonym(string word) const;
	size_t numSynonyms(string word);
	string filePath;
private:
	map<string, vector<string>> words;
};

void stringUpper(string &input);
bool wildcardMatch(const char *str, const char *strWild);
bool lineValid(string &line);
