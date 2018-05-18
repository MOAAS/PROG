#include "stdafx.h"
#include "Player.h"


Player::Player() {
	numClues = 0;
}

Player::Player(string name) {
	numClues = 0;
	this->name = name;
}

string Player::getName() const
{
	return name;
}

size_t Player::getNumClues()
{
	return numClues;
}

void Player::setName(string name) {
	this->name = name;
}

void Player::incClues()
{
	numClues++;
}

void Player::startClock() {
	startTime = time(NULL);
}

size_t Player::endClock() {
	endTime = time(NULL);
	return endTime - startTime;
}

