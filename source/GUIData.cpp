#include "GUIData.h" // interface
#include "GUI.h"
#include "App.h"
namespace gui {
	theme::theme() {}
	theme::theme(std::string name) {
		// TODO: (FEATURE) load theme from file
	}

	scaling::scaling(float _scale) {
		scale = _scale;

		boxBoundsScaled = (int)floor((float)boxConst * scale);

		topH = (int)floor((float)boxConst / 8.f * scale);
		boxW = (int)floor((float)boxConst * scale);
		boxH = (int)floor((float)boxConst * scale);
		space = (int)floor((float)spaceConst * scale);
		modSpace = (int)floor((float)spaceConst/2 * scale);
		pat = (int)floor((float)patConst * scale);
		patSpace = (int)floor((float)patSpaceConst * scale);
		patFull = pat + patSpace;
	}

	// default spacing
	scaling cs(1.f);
	theme th;
}