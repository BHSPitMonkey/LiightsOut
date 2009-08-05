/*
 *      liightsout.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Main source file for the Liights Out! game for the Wii.
 *      
 *      THANKS TO:
 * 		The guys from #wiidev
 * 		The devkitpro, libogc, libwiisprite, and other included libraries
 * 		Team Twiizers, for making it all possible
 */

#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <wiisprite.h>
#include <fat.h>
#include <vector>
#include <math.h>

#include <iostream>
#include <fstream>

#include "light.h"
#include "board.h"
#include "resourcemanager.h"
#include "menu.h"

// libwiisprite uses wsp as its namespace
using namespace wsp;

enum buttonPress {
	none=0,	// Nothing happened
	home,	// Or GC Start
	d_up,	// D-Pad
	d_down,	// D-Pad
	d_left,	// D-Pad
	d_right,// D-Pad
	a,	// A button
	b,	// B button
	minus,	// Or GC L
	plus,	// Or GC R
	one,	// Or GC Y
	two	// Or GC X
};

GameWindow gwd;			// Declare our drawing area object
ir_t ir0;			// Struct containing IR information
ir_t ir1;			// Struct containing IR information
ir_t ir2;			// Struct containing IR information
ir_t ir3;			// Struct containing IR information
int rumblecnt;			// Counter for rumble frames.
u64 frame = 0;			// Frame count

Sprite bg_sprite;		// Holds our background sprite
Sprite wave1_sprite;		// Holds the wave background sprite (first half)
Sprite wave2_sprite;		// Holds the wave background sprite (second half)
Sprite buttons_sprite;		// Holds our tiled buttons sprite
Sprite logo_big_sprite;		// Holds our big logo sprite
Sprite menu_list_sprite;	// Holds our main menu listing sprite
Sprite header_sprite;		// Holds the header sprite above the game board
Sprite freemode_sprite;		// Holds the "free mode" indicator sprite
Sprite thanks_sprite;		// Holds our Credits image
Sprite help_sprite;		// Holds the sprite on the Instructions page
Sprite pointer0_sprite;		// Holds our pointing hand cursor sprite
Sprite pointer0_shadow_sprite;	// Holds the shadow sprite for our pointing hand
Sprite pointer1_sprite;		// Holds our pointing hand cursor sprite
Sprite pointer1_shadow_sprite;	// Holds the shadow sprite for our pointing hand
Sprite pointer2_sprite;		// Holds our pointing hand cursor sprite
Sprite pointer2_shadow_sprite;	// Holds the shadow sprite for our pointing hand
Sprite pointer3_sprite;		// Holds our pointing hand cursor sprite
Sprite pointer3_shadow_sprite;	// Holds the shadow sprite for our pointing hand

Image bg_image;			// Holds our background image file
Image wave1_image;		// Holds the wave background image (first half)
Image wave2_image;		// Holds the wave background image (second half)
Image logo_big_image;		// Holds our big logo image
Image menu_list_image;		// Holds our menu list image
Image header_image;		// Holds our header image above the game board
Image freemode_image;		// Holds the "free mode" indicator image
Image thanks_image;		// Holds the Credits image
Image help1_image;		// Holds the first Help image
Image help2_image;		// Holds the second Help image
Image help3_image;		// Holds the third Help image
Image help_mote_image;		// Holds the mote Help image
Image help_cc_image;		// Holds the classic Help image
Image help_gcn_image;		// Holds the gamecube Help image
Image pointer0_image;		// Holds the pointing hand cursor image
Image pointer1_image;		// Holds the pointing hand cursor image
Image pointer2_image;		// Holds the pointing hand cursor image
Image pointer3_image;		// Holds the pointing hand cursor image
Image pointer_shadow_image;	// Holds the shadow image for our pointing hand

ResourceManager * theResourceManager;

bool free_mode = false;		// Setting enables individual light toggling

// Draws the cursor sprites at the point where the IR camera is aimed
void drawCursor() {

	// Detect wiimotes automatically
	WPAD_ScanPads();
	PAD_ScanPads();

	WPAD_IR(0, &ir0);	// Capture the first remote's IR to ir
	WPAD_IR(1, &ir1);	// Capture the first remote's IR to ir
	WPAD_IR(2, &ir2);	// Capture the first remote's IR to ir
	WPAD_IR(3, &ir3);	// Capture the first remote's IR to ir

	if ( ir3.valid ) {
		// Draw the shadow at an offset.
		pointer3_shadow_sprite.SetPosition(ir3.x - 48, ir3.y - 48 + 8);
		pointer3_shadow_sprite.SetRotation(ir3.angle/2);
		pointer3_shadow_sprite.Draw();
		// Draw the pointer hand on top of the shadow.
		pointer3_sprite.SetPosition(ir3.x - 48, ir3.y - 48);
		pointer3_sprite.SetRotation(ir3.angle/2);
		pointer3_sprite.Draw();
	}

	if ( ir2.valid ) {
		// Draw the shadow at an offset.
		pointer2_shadow_sprite.SetPosition(ir2.x - 48, ir2.y - 48 + 8);
		pointer2_shadow_sprite.SetRotation(ir2.angle/2);
		pointer2_shadow_sprite.Draw();
		// Draw the pointer hand on top of the shadow.
		pointer2_sprite.SetPosition(ir2.x - 48, ir2.y - 48);
		pointer2_sprite.SetRotation(ir2.angle/2);
		pointer2_sprite.Draw();
	}

	if ( ir1.valid ) {
		// Draw the shadow at an offset.
		pointer1_shadow_sprite.SetPosition(ir1.x - 48, ir1.y - 48 + 8);
		pointer1_shadow_sprite.SetRotation(ir1.angle/2);
		pointer1_shadow_sprite.Draw();
		// Draw the pointer hand on top of the shadow.
		pointer1_sprite.SetPosition(ir1.x - 48, ir1.y - 48);
		pointer1_sprite.SetRotation(ir1.angle/2);
		pointer1_sprite.Draw();
	}

	if ( ir0.valid ) {
		// Draw the shadow at an offset.
		pointer0_shadow_sprite.SetPosition(ir0.x - 48, ir0.y - 48 + 8);
		pointer0_shadow_sprite.SetRotation(ir0.angle/2);
		pointer0_shadow_sprite.Draw();
		// Draw the pointer hand on top of the shadow.
		pointer0_sprite.SetPosition(ir0.x - 48, ir0.y - 48);
		pointer0_sprite.SetRotation(ir0.angle/2);
		pointer0_sprite.Draw();
	}
}

// Draws the animated, wavy background
void drawWaveBG() {

	wave1_sprite.SetPosition( 320. * cos(frame/200.) - 320. , 208 );
	wave2_sprite.SetPosition( 320. * cos(frame/200.) + 320. , 208 );
	wave1_sprite.Draw();
	wave2_sprite.Draw();
	frame++;
}

// Fairly simplistic method that returns the currently pressed
// button as a buttonPress value (see the buttonPress enum).
// Note that this way of capturing input doesn't allow for
// button combinations.
buttonPress getButtonPress() {

	WPAD_ScanPads();
	PAD_ScanPads();

	// Define some shortcuts for button actions
	u32 mote1pressed	= WPAD_ButtonsDown(WPAD_CHAN_0);	// First Wiimote
	u16 pad1pressed		= PAD_ButtonsDown(PAD_CHAN0);		// First GC Pad

	// Listen for buttonpresses, and react to them

	// Home button or Start button
	if(( mote1pressed & WPAD_BUTTON_HOME ) || (mote1pressed & WPAD_CLASSIC_BUTTON_HOME) || ( pad1pressed & PAD_BUTTON_START ))
		return home;

	// D-Pads
	if(( mote1pressed & WPAD_BUTTON_UP ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_UP ) || ( pad1pressed & PAD_BUTTON_UP ))
		return d_up;
	if(( mote1pressed & WPAD_BUTTON_DOWN ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_DOWN ) || ( pad1pressed & PAD_BUTTON_DOWN ))
		return d_down;
	if(( mote1pressed & WPAD_BUTTON_LEFT ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_LEFT ) || ( pad1pressed & PAD_BUTTON_LEFT ))
		return d_left;
	if(( mote1pressed & WPAD_BUTTON_RIGHT ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_RIGHT ) || ( pad1pressed & PAD_BUTTON_RIGHT ))
		return d_right;

	// A button
	if(( mote1pressed & WPAD_BUTTON_A ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_A ) || ( pad1pressed & PAD_BUTTON_A ))
		return a;

	// B button
	if(( mote1pressed & WPAD_BUTTON_B ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_B ) || ( pad1pressed & PAD_BUTTON_B ))
		return b;

	// Plus and Minus, or the trigger buttons
	if(( mote1pressed & WPAD_BUTTON_PLUS ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_PLUS ) || ( pad1pressed & PAD_TRIGGER_R ))
		return plus;
	if(( mote1pressed & WPAD_BUTTON_MINUS ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_MINUS ) || ( pad1pressed & PAD_TRIGGER_L ))
		return minus;

	// One and Two, or GC Y and GC X, or Classic Y and Classic X
	if(( mote1pressed & WPAD_BUTTON_1 ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_Y ) || ( pad1pressed & PAD_BUTTON_Y ))
		return one;
	if(( mote1pressed & WPAD_BUTTON_2 ) || ( mote1pressed & WPAD_CLASSIC_BUTTON_X ) || ( pad1pressed & PAD_BUTTON_X ))
		return two;

	return none;
}

void quitToHBC() {

	exit(0);
}

// Displays the credits page
void credits() {

	while(1) {

		switch ( getButtonPress() ) {
			case a:
				return;		// Return to the game
			default: break;
		}

		bg_sprite.Draw();		// Draw the background

		drawWaveBG();			// Draw the animated wave in the background

		thanks_sprite.Draw();	// Draw the logo

//		WPAD_IR(0, &ir);		// Capture the first remote's IR to ir

		// if (ir.valid)			// If the IR pointer is valid...
			drawCursor();
		
		gwd.Flush();
	}
	
}

// Displays the instructions page
void help() {

	std::vector<Image*> help_imgs;
	help_imgs.push_back(&help1_image);
	help_imgs.push_back(&help2_image);
	help_imgs.push_back(&help3_image);
	
	u32 extension;

	// If the first Wii remote is active, show its Controls
	if (WPAD_Probe(WPAD_CHAN_0, &extension) == WPAD_ERR_NONE)
		help_imgs.push_back(&help_mote_image);
	// If the first Classic Controller is active, show its Controls
	if (WPAD_Probe(WPAD_CHAN_0, &extension) == WPAD_ERR_NONE && extension == WPAD_EXP_CLASSIC )
		help_imgs.push_back(&help_cc_image);
	// If the first Gamecube pad is active, show its Controls
//	if (PAD_Probe(WPAD_CHAN_0, WPAD_EXP_NONE) == WPAD_ERR_NONE)
		help_imgs.push_back(&help_gcn_image);
	
	unsigned short position = 0;
	help_sprite.SetImage( help_imgs.at(position) );
	help_sprite.SetPosition(320 - help_sprite.GetRefPixelX(), 
							240 - help_sprite.GetRefPixelY());

	while(1) {

		switch ( getButtonPress() ) {
			case a:	// Next
				if ( position >= help_imgs.size()-1 )
					return;
				else {
					position++;
					help_sprite.SetImage( help_imgs.at(position) );
					help_sprite.SetPosition(320 - help_sprite.GetRefPixelX(), 
											240 - help_sprite.GetRefPixelY());
				}
				break;
			case b:	// Back
				if ( position <= 0 )
					return;
				else {
					position--;
					help_sprite.SetImage( help_imgs.at(position) );
					help_sprite.SetPosition(320 - help_sprite.GetRefPixelX(), 
											240 - help_sprite.GetRefPixelY());
				}
				break;
			default: break;
		}

		bg_sprite.Draw();			// Draw the background

		drawWaveBG();				// Draw the animated wave background

		help_sprite.Draw();			// Draw the content

//		WPAD_IR(0, &ir);			// Capture the first remote's IR to ir

//		if (ir.valid)				// If the IR pointer is valid...
			drawCursor();
		
		gwd.Flush();
	}
	
}

// Displays the main menu
void menu() {

	Menu the_menu(	6, 320 - 106, 220,
					&menu_list_image, theResourceManager);

	while(1) {

		switch ( getButtonPress() ) {
			case home:
				quitToHBC();
			case a:
				// Act depending on which button is selected.
				switch (the_menu.getSelection()) {
					case 0:	// Play
						return;
					case 1:	// Levels
						break;
					case 2:	// Settings
						break;
					case 3:	// Instructions
						help();
						break;
					case 4:	// Credits
						credits();
						break;
					case 5:	// Quit
						exit(0);
					default: break;
				}
				break;
			case d_up:
				the_menu.moveSelection(previous);	break;	// Move selection up.
			case d_down:
				the_menu.moveSelection(next);		break;	// Move selection down.
			default: break;
		}

		bg_sprite.Draw();		// Draw the background

		drawWaveBG();			// Draw the animated wave in the background

		logo_big_sprite.Draw();	// Draw the logo
		
		the_menu.redraw();		// Draw the menu

		drawCursor();		// Draw the cursors

		// Select the Menu Button beneath the cursor
		the_menu.setSelectedUnderCursor(&pointer0_sprite);

		gwd.Flush();
	}
	
}

/* Main program method */

int main(int argc, char **argv)
{
	SYS_SetResetCallback(quitToHBC);	// Assign the Reset button to a function
	LayerManager manager(6);			// Manages layers and sprites.

	// Create the game window and initalise the VIDEO subsystem
	gwd.InitVideo();

	// Initialise Wiimote
	WPAD_Init();
	PAD_Init();
	
	// Detect wiimotes automatically
	WPAD_ScanPads();
	PAD_ScanPads();

	// Enable SD card stuff
	fatInitDefault();
	
	// Logging
	std::ofstream log;
	log.open ("/log.txt");


	// Load the background sprite/texture
	if(bg_image.LoadImage("/apps/liightsout/gfx/bg.png") != IMG_LOAD_ERROR_NONE) exit(0);
	bg_sprite.SetImage(&bg_image);
	bg_sprite.SetPosition(0, 0);

	if(wave1_image.LoadImage("/apps/liightsout/gfx/wave1.png") != IMG_LOAD_ERROR_NONE);
	wave1_sprite.SetImage(&wave1_image);
	if(wave2_image.LoadImage("/apps/liightsout/gfx/wave2.png") != IMG_LOAD_ERROR_NONE);
	wave2_sprite.SetImage(&wave2_image);

	// Create a simple pointer...
	if(pointer0_image.LoadImage("/apps/liightsout/gfx/player1_point.png") != IMG_LOAD_ERROR_NONE);// exit(0);
	pointer0_sprite.SetImage(&pointer0_image);
	pointer0_sprite.DefineCollisionRectangle(47, 47, 1, 1);	// Define the subsection of the pointer we watch for collisions
	if(pointer_shadow_image.LoadImage("/apps/liightsout/gfx/shadow_point.png") != IMG_LOAD_ERROR_NONE) exit(0);
	pointer0_shadow_sprite.SetImage(&pointer_shadow_image);

	// Create other pointers...
	if(pointer1_image.LoadImage("/apps/liightsout/gfx/player2_point.png") != IMG_LOAD_ERROR_NONE) exit(0);
	pointer1_sprite.SetImage(&pointer1_image);
	pointer1_sprite.SetTransparency(128);
	pointer1_shadow_sprite.SetImage(&pointer_shadow_image);
	pointer1_shadow_sprite.SetTransparency(96);

	if(pointer2_image.LoadImage("/apps/liightsout/gfx/player3_point.png") != IMG_LOAD_ERROR_NONE) exit(0);
	pointer2_sprite.SetImage(&pointer2_image);
	pointer2_sprite.SetTransparency(128);
	pointer2_shadow_sprite.SetImage(&pointer_shadow_image);
	pointer2_shadow_sprite.SetTransparency(96);

	if(pointer3_image.LoadImage("/apps/liightsout/gfx/player4_point.png") != IMG_LOAD_ERROR_NONE) exit(0);
	pointer3_sprite.SetImage(&pointer3_image);
	pointer3_sprite.SetTransparency(128);
	pointer3_shadow_sprite.SetImage(&pointer_shadow_image);
	pointer3_shadow_sprite.SetTransparency(96);


	// Initialize some graphics
	if(header_image.LoadImage("/apps/liightsout/gfx/header.png") != IMG_LOAD_ERROR_NONE) exit(0);
	header_sprite.SetImage(&header_image);
	header_sprite.SetPosition(320 - header_sprite.GetRefPixelX(), 35);		// Center along X
	if(freemode_image.LoadImage("/apps/liightsout/gfx/freemode.png") != IMG_LOAD_ERROR_NONE) exit(0);
	freemode_sprite.SetImage(&freemode_image);
	freemode_sprite.SetPosition(75, 129);		// Center along X

	// Initialize the main menu's graphics
	if(logo_big_image.LoadImage("/apps/liightsout/gfx/logo_big.png") != IMG_LOAD_ERROR_NONE) exit(0);
	logo_big_sprite.SetImage(&logo_big_image);
	logo_big_sprite.SetPosition(320 - logo_big_sprite.GetRefPixelX(), 78);	// Center along X
	if(menu_list_image.LoadImage("/apps/liightsout/gfx/menu_list.png") != IMG_LOAD_ERROR_NONE) exit(0);
	menu_list_sprite.SetImage(&menu_list_image);
	menu_list_sprite.SetPosition(320 - menu_list_sprite.GetRefPixelX(), 220);	// Center along X

	// Initialize the Instructions page graphics
	if(help1_image.LoadImage("/apps/liightsout/gfx/help1.png") != IMG_LOAD_ERROR_NONE) exit(0);
	if(help2_image.LoadImage("/apps/liightsout/gfx/help2.png") != IMG_LOAD_ERROR_NONE) exit(0);
	if(help3_image.LoadImage("/apps/liightsout/gfx/help3.png") != IMG_LOAD_ERROR_NONE) exit(0);
	if(help_mote_image.LoadImage("/apps/liightsout/gfx/help-mote.png") != IMG_LOAD_ERROR_NONE) exit(0);
	if(help_cc_image.LoadImage("/apps/liightsout/gfx/help-cc.png") != IMG_LOAD_ERROR_NONE) exit(0);
	if(help_gcn_image.LoadImage("/apps/liightsout/gfx/help-gcn.png") != IMG_LOAD_ERROR_NONE) exit(0);

	// Initialize the Credits page graphics
	if(thanks_image.LoadImage("/apps/liightsout/gfx/thanks.png") != IMG_LOAD_ERROR_NONE);
	thanks_sprite.SetImage(&thanks_image);
	thanks_sprite.SetPosition(320 - thanks_sprite.GetRefPixelX(), 240 - thanks_sprite.GetRefPixelY());	// Center

	// IR initialization
	WPAD_SetVRes(WPAD_CHAN_ALL,640,480);
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetDataFormat(WPAD_CHAN_1, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetDataFormat(WPAD_CHAN_2, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetDataFormat(WPAD_CHAN_3, WPAD_FMT_BTNS_ACC_IR);


	theResourceManager = new ResourceManager();	// Caches common Image textures

	menu();			// Open the main menu.

	Board theBoard(theResourceManager);	// Manages the lights and their actions in the game

	/* Main program loop */
	while(1)
	{
		switch ( getButtonPress() ) {
			case none:							break;
			case home:
				menu();							break;
			case a:
				if (!free_mode)
					theBoard.toggleAtSelection();			// Make a move.
				else theBoard.toggleAtSelection(true);		// Toggle the individual light.
				if( theBoard.isSolved() ) {					// See if they've won.
					WPAD_Rumble(WPAD_CHAN_0, 1);			// If so, do something.
					rumblecnt = 10;
				}								break;
			case d_up:
				theBoard.moveSelection(0);		break;	// Move selection up.
			case d_down:
				theBoard.moveSelection(1);		break;	// Move selection down.
			case d_left:
				theBoard.moveSelection(2);		break;	// Move selection left.
			case d_right:
				theBoard.moveSelection(3);		break;	// Move selection right.
			case plus:
				theBoard.loadPuzzle(-1);		break;	// Go down one level.
			case minus:
				theBoard.loadPuzzle(-2);		break;	// Go up one level.
			case two:
				if(free_mode == false) free_mode = true;
					else free_mode = false;		break;	// Free Mode setting

			default:							break;	// Do nothing.	
		}

		// Stupid rumble controller logic
		if(rumblecnt > 1)
			rumblecnt--;
		else if (rumblecnt == 1) {
			rumblecnt--;
			WPAD_Rumble(WPAD_CHAN_0, 0);	// Disable motor
			PAD_ControlMotor(PAD_CHAN0, PAD_MOTOR_STOP);
		}

		bg_sprite.Draw();			// Draw the background

		drawWaveBG();				// Draw the animated wave in the background

		header_sprite.Draw();			// Draw the header

		if (free_mode) freemode_sprite.Draw();	// Draw the "free mode" symbol if free mode is on

		theBoard.redraw();			// Draw the game board

		drawCursor();

		// Select the Light beneath the cursor.
		// This "if" statement used to increment rumblecnt when evaluated to be true.
		// I never got the interface rumbling feature working to my liking, so I removed it.
		if (theBoard.setSelectedUnderCursor(&pointer0_sprite) && rumblecnt==0);

		gwd.Flush();
	}
	return 0;
}
