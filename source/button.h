/*
 *      Button.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Light objects.  A Light represents one of the
 * 		buttons on a Lights Out puzzle board.  It knows its state
 * 		and where it should be drawn on the screen.
 */

#include <wiisprite.h>
#include "resourcemanager.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
// constructors and destructor:

	void initUsing(int x_coord, int y_coord, ResourceManager* rm);	
	// initializer which takes the state, position coordinates, and a resourcemanager

	Button();						// default constructor
	~Button();						// destructor

// Button operations:

	void setSelected(bool setting);
	// Sets the button as being "selected"

	wsp::Sprite * getSprite();
	// Returns a pointer to this light's sprite

	void draw();
	// Redraws the button on the screen.
	

private:

   bool selected;	// true if this light is being selected by a cursor.

   ResourceManager * resources;	// The resourcemanager where textures are accessed

   wsp::Sprite button_sprite;	// This button's drawing unit.

};  // end class

#endif
