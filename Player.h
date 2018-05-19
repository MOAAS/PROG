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
	bool isNormalMode() const;
	void setName(string name);
	void setEasyMode();
	void incClues(); // soma 1 ao número de clues
	void startClock();
	size_t endClock(); // retorna o tempo gasto
private:
	bool normalMode; // false = easy mode.
	string name;
	size_t startTime;
	size_t endTime;
	size_t numClues;
};
