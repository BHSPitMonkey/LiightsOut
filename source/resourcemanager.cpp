#include "resourcemanager.h"
#include <wiisprite.h>

using namespace wsp;

// Constructor.  The value passed to images() is the size of our Vector.
ResourceManager::ResourceManager() : images(7) {
	
	// Load all of our commonly-used Images (textures)

	if(images.at(light_on).LoadImage("/apps/liightsout/gfx/light_on.png") != IMG_LOAD_ERROR_NONE) 				;// exit(0);
	if(images.at(light_on_hover).LoadImage("/apps/liightsout/gfx/light_on_hover.png") != IMG_LOAD_ERROR_NONE)	;// exit(0);
	if(images.at(light_on_down).LoadImage("/apps/liightsout/gfx/light_on_hover.png") != IMG_LOAD_ERROR_NONE)	;// exit(0);
	if(images.at(light_off).LoadImage("/apps/liightsout/gfx/light_off.png") != IMG_LOAD_ERROR_NONE)				;// exit(0);
	if(images.at(light_off_hover).LoadImage("/apps/liightsout/gfx/light_off_hover.png") != IMG_LOAD_ERROR_NONE)	;// exit(0);
	if(images.at(light_off_down).LoadImage("/apps/liightsout/gfx/light_off_down.png") != IMG_LOAD_ERROR_NONE)	;// exit(0);
	if(images.at(button_hover).LoadImage("/apps/liightsout/gfx/button_hover.png") != IMG_LOAD_ERROR_NONE)		;// exit(0);
}

// Getter method, returns a reference to the requested Image img.
// Acceptable values for img are defined in the enum in the header file.
Image& ResourceManager::getImage(ImageName img) {
	return images.at(img);
}
