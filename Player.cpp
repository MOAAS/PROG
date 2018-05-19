#include "stdafx.h"
#include "Player.h"


Player::Player() {
	normalMode = true;
	numClues = 0;
}

Player::Player(string name) {
	normalMode = true;
	numClues = 0;
	this->name = name;
}

string Player::getName() const {
	return name;
}

size_t Player::getNumClues() const {
	return numClues;
}

size_t Player::getTimeTaken() const {
	return endTime - startTime;
}

bool Player::isNormalMode() const {
	return normalMode;
}

void Player::setName(string name) {
	this->name = name;
}

void Player::setEasyMode() {
	normalMode = false;
}

void Player::incClues() {
	numClues++;
}

void Player::startClock() {
	startTime = time(NULL);
}


void Player::endClock() {
	endTime = time(NULL);
}
