/*
 *      Board.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 	Class header for Board objects.  A Board represents a "Lights
 * 	Out" puzzle, and contains 25 Light objects.  The class can
 * 	perform actions upon itself such as making a move at a
 * 	specified spot, or testing whether it is in a 'solved' state.
 */

#include "light.h"
#include "board.h"  // header file
#include <fat.h>
#include <iostream>
#include <fstream>

using namespace wsp;

// This makes Puzzle objects accessible like arrays. Or something.
// The Puzzle class is defined in board.h for now, if you were curious.
bool * Puzzle::operator[](int i) {
	if (i >= 0 && i < 5)
		return b[i];
	else return b[0];
}

// constructor
Board::Board(ResourceManager* resourcemanager) {

	// Read puzzles from text file into the puzzles 
	std::ifstream myfile ("/apps/liightsout/data/puzzles.txt");
	Puzzle tmp;
	if (myfile.is_open()) {
		while ( myfile >>	tmp[0][0] >> tmp[1][0] >> tmp[2][0] >> tmp[3][0] >> tmp[4][0] >> 
					tmp[0][1] >> tmp[1][1] >> tmp[2][1] >> tmp[3][1] >> tmp[4][1] >> 
					tmp[0][2] >> tmp[1][2] >> tmp[2][2] >> tmp[3][2] >> tmp[4][2] >> 
					tmp[0][3] >> tmp[1][3] >> tmp[2][3] >> tmp[3][3] >> tmp[4][3] >> 
					tmp[0][4] >> tmp[1][4] >> tmp[2][4] >> tmp[3][4] >> tmp[4][4] ) {
			puzzles.push_back(tmp);
		}
		myfile.close();
	} 

	// Initialize the array of lights, all in the "on" state
	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			lights[i][j].initUsing(true, 192+i*72, 90+j*72, resourcemanager);

	selected_col = 0;  selected_row = 0;
	lights[selected_col][selected_row].setSelected(true);

	loadPuzzle(0);		// Load the first level.
}

// destructor
Board::~Board() { }	

// Returns a boolean indicating whether the puzzle is solved (all of the Lights must be off).
bool Board::isSolved() {
	// Return false if we find any lights turned on
	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			if ( lights[i][j].isOn() )
				return false;

	// If we didn't find any lights on, then the board is solved.
	return true;
}

// Returns a reference to the Light object at the requested row and column.
Light & Board::lightAtPosition(unsigned short col, unsigned short row) {

	if ((col > 4) || (row > 4)) {	// If the input is unreasonable
		return lights[0][0];	// This is not a good solution!
	}

	return lights[col][row];
}	// NOTE: Columns start at left, Rows start at top

// Toggles the selected light, as well as its four adjacent Lights.
void Board::toggleAtSelection(bool free_mode) {
	
	selected_row = selected_row % 5;	// bring the selected row into a reasonable range
	selected_col = selected_col % 5;	// bring the selected col into a reasonable range
	
	lights[selected_col][selected_row].toggle();
	
	if (free_mode) return;				// if Free Mode is enabled, stop here.
	
	if (selected_col > 0)
		lights[selected_col-1][selected_row].toggle();

	if (selected_col < 4)
		lights[selected_col+1][selected_row].toggle();

	if (selected_row > 0)
		lights[selected_col][selected_row-1].toggle();

	if (selected_row < 4)
		lights[selected_col][selected_row+1].toggle();
}

// Prints the current board state to standard output for debugging
void Board::print() {

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			if (lights[i][j].isOn()) std::cout << "ON  ";
			else std::cout << "OFF ";
		}
		std::cout << std::endl;
	}
}

// Moves the selected light cursor to an adjacent position. (The D-Pad does this.)
// Pass direction as 0 for up, 1 for down, 2 for left, 3 for right
void Board::moveSelection(short direction) {
	
	// Deselect existing selection
	lights[selected_col % 5][selected_row % 5].setSelected(false);

	if (direction == 0) {	// up
		if (selected_row == 0) selected_row = 4;
		else selected_row--;
	}
	if (direction == 1)		// down
		selected_row++;
	if (direction == 2)	{	// left
		if (selected_col == 0) selected_col = 4;
		else selected_col--;
	}
	if (direction == 3)		// right
		selected_col++;

	// Select new selection
	lights[selected_col % 5][selected_row % 5].setSelected(true);
}

// Moves the selected light cursor to a specified row and column. (The IR cursor does this.)
void Board::setSelection(unsigned short col, unsigned short row) {

	// Deselect existing selection
	lights[selected_col % 5][selected_row % 5].setSelected(false);

	// Fix out-of-range values
	selected_col = col % 5;
	selected_row = row % 5;

	// Select new selection
	lights[selected_col % 5][selected_row % 5].setSelected(true);
}

// Sets the selection to whatever is beneath the IR cursor.
// Return true if this is a NEW selection, false if there is no change.
bool Board::setSelectedUnderCursor(wsp::Sprite * cursor_sprite) {

	// Loop through each Light, asking it if it collides with the cursor.
	// If a Light DOES collide, and that Light was not already selected, we select it.
	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			if (lights[i][j].getSprite()->CollidesWith(cursor_sprite)) {
				if (( selected_col%5 != i ) || ( selected_row%5 != j ))	 { // If this is new
					setSelection(i, j);	// Set the new selection
					return true;
				}
			}

	// Nothing interesting happened. Return false.
	return false;
}

// Draw the board.
void Board::redraw() {

	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			lights[i][j].draw();
}

// Load a puzzle configuration onto the board.
int Board::loadPuzzle(int puzzleNumber) {

	if (puzzleNumber == -2)	{				// The user wants to go down a level
		if (currentLevel > 0)					// If we have room to move down...
			puzzleNumber = currentLevel-1;
	}
	else if (puzzleNumber == -1) {			// The user wants to go up a level
		if (currentLevel < puzzles.size()-1)		// If we have room to move up...
			puzzleNumber = currentLevel+1;
	}

	if ((puzzleNumber >= 0) && (puzzleNumber < puzzles.size())) {	// If input is sane
		
		for (int i=0; i<5; i++)
			for (int j=0; j<5; j++)
				lights[i][j].setState( puzzles.at(puzzleNumber)[i][j] );
		
		currentLevel = puzzleNumber;		// Remember where we're at.
	}
	
	// else do nothing
	
	return currentLevel;
}
