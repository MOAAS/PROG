#include "stdafx.h"
#include "Cursor.h"

Cursor::Cursor(int x, int y, char dir) {
	this->x = x;
	this->y = y;
	this->dir = dir;
}

void Cursor::moveTo(int x, int y) {
	this->x = x;
	this->y = y;
}

void Cursor::moveTo(int x, int y, char dir) {
	this->x = x;
	this->y = y;
	this->dir = dir;
}

void Cursor::changeDir(char dir) {
	this->dir = dir;
}

Cursor Cursor::operator++(int unused) {
	switch (dir) {
	case 'H': x++; break;
	case 'V': y++; break;
	}
}
Cursor Cursor::operator--(int unused) {
	switch (dir) {
	case 'H': x--; break;
	case 'V': y--; break;
	}
}

