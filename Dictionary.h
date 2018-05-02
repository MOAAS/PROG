#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

class Dictionary {
public:
	Dictionary();
	void load(string textFile);
	bool wordExists(string word) const;
	void display() const;
	vector<string> getWildcardMatches(string wildcardWord) const;
private:
	map<string, vector<string>> words;
};

void stringUpper(string &input);
void alphaOnly(string &str);