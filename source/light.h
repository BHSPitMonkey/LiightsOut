/*
 *      Light.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Light objects.  A Light represents one of the
 * 		buttons on a Lights Out puzzle board.  It knows its state
 * 		and where it should be drawn on the screen.
 */

#include <wiisprite.h>
#include "resourcemanager.h"

class Light {
public:
// constructors and destructor:

	void initUsing(bool theState, int x_coord, int y_coord, ResourceManager* rm);	
	// initializer which takes the state, position coordinates, and a resourcemanager

	Light();						// default constructor
	~Light();						// destructor

// Light operations:

	bool isOn();
	// Returns whether the light is on or off.
	
	void toggle();
	// Flips the current state.  The button is animated/redrawn
	// in this method as well.

	void setState(bool setting);
	// Sets the light's state directly

	void setSelected(bool setting);
	// Sets the light as being "selected"

	wsp::Sprite * getSprite();
	// Returns a pointer to this light's sprite

	void draw();
	// Redraws the light on the screen.
	

private:

   int x_coord;		// X position on the screen where this light is drawn.
   int y_coord;		// Y position on the screen where this light is drawn.
   bool state;		// true if the light is on, false if it is off.
   bool selected;	// true if this light is being selected by a cursor.
   
   ResourceManager * resources;
   
   wsp::Sprite button_sprite;
 //  wsp::Image light_img_on;
 //  wsp::Image light_img_off;
   
  // wsp::Rectangle theRect;
  // wsp::Quad theQuad;

};  // end class
