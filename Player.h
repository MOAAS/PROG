#pragma once
#include <string>
#include <ctime>
#include <map>
#include <vector>
using namespace std;
class Player
{
public:
	Player();
	Player(string name);
	string getName() const;
	size_t getNumClues();
	void setName(string name);
	void incClues();
	void startClock();
	size_t endClock();
private:
	string name;
	size_t startTime;
	size_t endTime;
	size_t numClues;
	map<string, vector<string>> knownSynonyms;
};

