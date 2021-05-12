#include "GUIData.h" // interface
#include "App.h"
namespace gui {
	theme::theme() {}
	theme::theme(std::string name) {
		// TODO: (FEATURE) load theme from file
	}

	scaling::scaling(float Scale) {
		scale = Scale;
		boxBoundsScaled = (int)floor((float)boxConst * scale);
		boxW = (int)floor((float)boxConst * scale);
		boxH = (int)floor((float)boxConst * scale);
		space = (int)floor((float)spaceConst * scale);
		pat = (int)floor((float)patConst * scale);
		patSpace = (int)floor((float)patSpaceConst * scale);
		patFull = pat + patSpace;
	}

	// default spacing
	scaling cs(1.f);
	theme th;
}