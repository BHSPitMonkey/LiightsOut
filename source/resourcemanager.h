#include <vector>
#include <wiisprite.h>

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {

	public:

		// Defines the names of images we will use.
		enum ImageName {
			light_on,
			light_on_hover,
			light_on_down,
			light_off,
			light_off_hover,
			light_off_down,
			button_hover
		};

		ResourceManager();
		// Constructor

		wsp::Image& getImage(ImageName img);
		// Getter method, returns a reference to the requested Image img.
		// Acceptable values for img are defined in the enum in the header file.

	private:

		// Our vector of images.
		std::vector<wsp::Image> images;

}; // end ResourceManager class

#endif
