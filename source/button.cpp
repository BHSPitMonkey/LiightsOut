/*
 *      Light.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class for Light objects.  A Light represents one of the
 * 		buttons on a Lights Out puzzle board.  It knows its state
 * 		and where it should be drawn on the screen.
 */

#include <wiisprite.h>
#include "button.h"  // header file
#include "resourcemanager.h"

using namespace wsp;

void Button::initUsing(int x, int y, ResourceManager* rm) {

	selected = false;					// Initialize the selected-ness

	button_sprite.SetPosition(x, y);	// Tell the sprite where to draw itself
	
	resources = rm;						// Remember where the Resource Manager lives.
	
	button_sprite.SetImage( & resources->getImage(ResourceManager::button_hover) );
}

// Default constructor
Button::Button() { }

// Destructor
Button::~Button() { }


/* Button operations: */

// Sets the button selected-ness
void Button::setSelected(bool setting) {

	selected = setting;
}

// Returns a pointer to the sprite.
Sprite * Button::getSprite() {
	return &button_sprite;
}

// Draws the button
void Button::draw() {

	if (selected)
		button_sprite.Draw();

}
