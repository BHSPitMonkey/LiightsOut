/*
 *      Menu.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Menu objects.  A Menu manages one or more
 * 		"Button" objects, and allows for a selection cursor to move
 * 		about them.
 */

#include "resourcemanager.h"
#include "button.h"
#include <wiisprite.h>
#include <fstream>

#ifndef MENU_H
#define MENU_H

enum Orientation {
	vertical,
	horizontal
};

enum Direction1D {
	previous,
	next
};

class Menu {
public:

	// constructors and destructor:
	Menu(short indices, short x, short y, wsp::Image * labels, ResourceManager* resourcemanager,
			Orientation direction = vertical, unsigned short buttonSpacing = 0);
	~Menu();		// destructor

	/* Menu operations: */
	
	Button & buttonAtPosition(unsigned short index);
	// Returns a reference to the Light object at an index.

	void moveSelection(Direction1D direction);
	// Used to change the selected light to an adjacent one.
	// Set direction to 0 for prev, 1 for next.

	void setSelection(unsigned short index);
	// Set selection directly.
	
	short getSelection();
	// Get the current selection index.

	bool setSelectedUnderCursor(wsp::Sprite * cursor_sprite);
	// Selects the index beneath the cursor.
	// Returns true iff a new selection is noticed.

	void redraw();
	// Redraws each light.

private:

	bool isVertical;			// True if the buttons are stacked vertically, false otherwise
	short num_buttons;			// The number of buttons
	Button * buttons;			// The pointer to an array of Buttons we will dynamically allocate later
	wsp::Sprite labels_sprite;	// The sprite that draws the menu's backdrop/labels
	short selected_index;		// Which index is currently selected
	std::ofstream mylog;		// For logging purposes only... Should be removed upon cleaning

};  // end class

#endif
