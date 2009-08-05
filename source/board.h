/*
 *      Board.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Board objects.  A Board represents a "Lights
 * 		Out" puzzle, and contains 25 Light objects.  The class can
 * 		perform actions upon itself such as making a move at a
 * 		specified spot, or testing itself for having been solved.
 */

#include "resourcemanager.h"
#include <wiisprite.h>

#ifndef BOARD_H
#define BOARD_H

class Puzzle {
	private:
		bool b[5][5];
	public:
		bool * operator[](int i);
};

class Board {
public:
// constructors and destructor:
	Board(ResourceManager* resourcemanager);	// default constructor
	~Board();		// destructor

// Board operations:

	bool isSolved();
	// Returns whether the board is solved (whether all its
	// lights are off).
	
	Light & lightAtPosition(unsigned short x, unsigned short y);
	// Returns a reference to the Light object at (x,y), using
	// the following coordinate system:	  0 1 2 3 4
	//									0 x x x x x
	//									1 x x x x x
	//										and so on.

	void toggleAtSelection(bool free_mode = false);
	// Toggles the selected light, as well as its four adjacent Lights.
	// Optional boolean parameter enables Free Mode, false by default.

	void moveSelection(short direction);
	// Used to change the selected light to an adjacent one.
	// Set direction to 0 for up, 1 for down, 2 for left, 3 for right

	void setSelection(unsigned short col, unsigned short row);
	// Set selection directly.

	bool setSelectedUnderCursor(wsp::Sprite * cursor_sprite);
	// Selects the row/column beneath the cursor.
	// Returns true iff a new selection is noticed.

	void redraw();
	// Redraws each light.
	
	int loadPuzzle(int puzzleNumber);
	// Loads a puzzle configuration, returns the current level.

	void print();

private:

	Light lights[5][5];

	std::vector<Puzzle> puzzles;

	int selected_row;
	int selected_col;

	unsigned int currentLevel;

};  // end class

#endif
