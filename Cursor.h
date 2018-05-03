#pragma once
class Cursor {
public:
	friend class Board;
	Cursor();
	Cursor(int x, int y, char dir);
	void moveTo(int x, int y);
	void moveTo(int x, int y, char dir);
	void changeDir(char dir);
	Cursor operator++(int unused);
	Cursor operator--(int unused);
	int CursorMainCoord(); //da a coordenada principal na direcao atual
private:
	int x, y;
	char dir; // Direção. H / V
};
