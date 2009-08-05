/*
 *      Light.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class for Light objects.  A Light represents one of the
 * 		buttons on a Lights Out puzzle board.  It knows its state
 * 		and where it should be drawn on the screen.
 */

// #include <wiisprite.h>
#include "light.h"  // header file
#include "resourcemanager.h"

using namespace wsp;

void Light::initUsing(bool theState, int x, int y, ResourceManager* rm) {

	state = theState;					// Initialize the state
	selected = false;					// Initialize the selected-ness
	x_coord = x;						// Remember where to draw this Light.
	y_coord = y;						// Remember where to draw this Light.

	button_sprite.SetPosition(x, y);	// Tell the sprite where to draw itself
	
	resources = rm;						// Remember where the Resource Manager lives.
}

// Default constructor
Light::Light() { }

// Destructor
Light::~Light() { }


// Light operations:

// Returns whether the light is on or off.
bool Light::isOn() {
	return state;
}

// Flips the current state and redraws the button to show it.
void Light::toggle() {

	if ( state == true ) {	// if it was on
		state = false;		// turn it off
	} else {				// if it was off
		state = true;		//turn it on
	}
}

// Sets the light's state directly
void Light::setState(bool setting) {

	state = setting;
}

// Sets the light as being "selected"
void Light::setSelected(bool setting) {

	selected = setting;
}

// Returns a pointer to the sprite.
Sprite * Light::getSprite() {
	return &button_sprite;
}

// Draws the button
void Light::draw() {

	if (state && !selected)			// On and not selected
		button_sprite.SetImage( & resources->getImage(ResourceManager::light_on) );
	else if (state && selected)		// On and selected
		button_sprite.SetImage( & resources->getImage(ResourceManager::light_on_hover) );
	else if (!state && !selected)	// Off and not selected
		button_sprite.SetImage( & resources->getImage(ResourceManager::light_off) );
	else if (!state && selected)	// Off and selected
		button_sprite.SetImage( & resources->getImage(ResourceManager::light_off_hover) );

	button_sprite.Draw();

}
