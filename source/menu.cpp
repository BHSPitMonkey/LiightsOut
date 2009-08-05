/*
 *      Menu.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Menu objects.  A Menu manages one or more
 * 		"Button" objects, and allows for a selection cursor to move
 * 		about them.
 */
#include "button.h"
#include "menu.h"  // header file
#include <iostream>
#include <fstream>
#include <fat.h>

using namespace wsp;

// constructor
Menu::Menu(short indices, short x, short y, Image* labels, ResourceManager* resourcemanager,
			Orientation direction, unsigned short buttonSpacing) {

	labels_sprite.SetImage(labels);
	labels_sprite.SetPosition(x, y);

	// Set the requested orientation for the menu
	if (direction == vertical) isVertical = true;
	else isVertical = false;

	num_buttons = indices;				// Store the number of buttons to use
	buttons = new Button[num_buttons];	// Dynamically allocate an array of Buttons

	// Initialize the array of buttons.  Set their distance apart from 
	// one another based on the dimensions of the button_hover Image.
	for (int i=0; i<num_buttons; i++)
		if (isVertical)
		buttons[i].initUsing(	x,
								y + i * resourcemanager->getImage(ResourceManager::button_hover).GetHeight() + buttonSpacing,
								resourcemanager);
		else
		buttons[i].initUsing(	x + i * resourcemanager->getImage(ResourceManager::button_hover).GetWidth() + buttonSpacing,
								y,
								resourcemanager);

	// Select the first Button initially
	selected_index = 0;
	buttons[selected_index].setSelected(true);

}

// destructor
Menu::~Menu() { }	

// Returns a reference to the Button at a given position in the menu.
Button & Menu::buttonAtPosition(unsigned short index) {

	if ((index > num_buttons-1)) {	// If the input is unreasonable, then, uh,
		return buttons[0];			// just give the caller the first button. 
	}

	return buttons[index];			// Otherwise, give then the button they want.
}

// Used to change the selected button to an adjacent one.
// Set direction to 0 for previous, 1 for next
void Menu::moveSelection(Direction1D direction) {
	
	// Deselect existing selection
	buttons[selected_index % num_buttons].setSelected(false);

	switch (direction) {
		case previous:
			if (selected_index == 0) selected_index = num_buttons-1;
			else selected_index--;
			break;
		case next:
			selected_index++;
			break;
		default: break;
	}
/*
	if (direction == 0) {	// up
		if (selected_index == 0) selected_index = num_buttons-1;
		else selected_index--;
	}
	if (direction == 1)		// down
		selected_row++;
*/
	// Select new selection
	buttons[selected_index % num_buttons].setSelected(true);
}

// Changes the selection directly.
void Menu::setSelection(unsigned short index) {

	// Deselect existing selection
	buttons[selected_index % num_buttons].setSelected(false);

	selected_index = index % num_buttons;

	// Select new selection
	buttons[selected_index % num_buttons].setSelected(true);
}

// Get the current selection index.
short Menu::getSelection() {
	return selected_index%num_buttons;
}

// Sets the selection to whatever is beneath the IR cursor.
// Return true if this is a NEW selection.
bool Menu::setSelectedUnderCursor(wsp::Sprite * cursor_sprite) {

	// Logging
	//mylog.open ("/log.txt", std::ios::app);

	for (int i=0; i<num_buttons; i++)
		if (buttons[i].getSprite()->CollidesWith(cursor_sprite)) {
			//mylog << "Cursor collides with button " << i << std::endl;
			if ( selected_index%num_buttons != i )	 { // If this is new
				setSelection(i);		// Set the new selection
				//mylog << "Setting selection to button " << i << std::endl;
				//mylog.close();
				return true;
			}
		}
	//mylog.close();
	return false;	// If nothing of interest happens.
}

void Menu::redraw() {

	for (int i=0; i<num_buttons; i++)
		buttons[i].draw();

	labels_sprite.Draw();
}
