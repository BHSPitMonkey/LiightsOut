Liights Out!
============

A homebrew game for the Nintendo Wii

By Stephen Eisenhauer (BHSPitMonkey)  
mail@stepheneisenhauer.com


Description
-----------

Liights Out! is a puzzle game for the Wii, based on the classic puzzle 
game Lights Out.

The goal is to turn off all the lights on the board. When you toggle 
one light, the lights nearby also toggle.

Liights Out! is written in C++ and uses:

 -	libogc for Wii hardware support
 -	libwiisprite for high-level graphics rendering 

If you come up with some cool changes/features and you'd like to see 
them merged, feel free to send me a merge request.


Compiling from Source
---------------------

In order to compile this software, you must have

 -	DevkitPPC
 -	libogc
 -	libwiisprite

properly installed. See their respective sites and 
documentation for help.

To compile, simply run the 'make' command in this directory.

Further information about Wii homebrew development is available 
at http://wiibrew.org.


Installation
------------

To run this software after compilation:

 -	Copy the contents of this directory into /apps/liightsout/ on 
	your SD card.
	 -	Note that this location is hard-coded into the program, so 
		you MUST use this exact location. Sorry.
 -	Launch the game, "Liights Out!", using the Homebrew Channel.
 
(The 'source' and 'build' directories do not need to be copied 
to the SD card.)

You should have a directory hierarchy on your SD card similar to this:

	/apps/
	/apps/liightsout/
	/apps/liightsout/data/
	/apps/liightsout/gfx/
	/apps/liightsout/source/


Links
-----

For more information about Liights Out!, check out these links:

 -	[Liights Out page on Wiibrew.org](http://wiibrew.org/wiki/LiightsOut)
 -	[Developer's home page](http://stepheneisenhauer.com)


Credits and License
-------------------

Liights Out! was developed by Stephen Eisenhauer. Its source code is
made available under the MIT License, and its graphical resources/artwork
are made available under a Creative Commons CC-BY license.
