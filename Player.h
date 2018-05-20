#pragma once
#include <string>
#include <ctime>
using namespace std;
class Player
{
public:
	Player(); // inicializa com normalMode = true, numClues = 0.
	Player(string name); // inicializa com um nome também.
	string getName() const;
	size_t getNumClues() const;
	size_t getTimeTaken() const; // retorna o tempo gasto
	bool isNormalMode() const;
	void setName(string name);
	void setEasyMode();
	void incClues(); // soma 1 ao número de clues
	void startClock();
	void endClock();
private:
	bool normalMode; // false = easy mode.
	string name;
	time_t startTime;
	time_t endTime;
	size_t numClues;
};

