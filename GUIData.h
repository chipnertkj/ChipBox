#ifndef GUIDATA_H
#define GUIDATA_H


// structs, enums
namespace gui {
	// ui adjustment
	struct spacing {
		float scale = 1.f;
		const static int boxDefault = 240;

		int boxW = boxDefault;
		int boxH = boxDefault;
		int space = 6;
		int pat = 30;
		int patSpace = 3;
		int patFull = pat + patSpace;

		spacing(float _scale);
	};

	// usage : theme[c::bg] NOT theme[1]
	// for code readability
	enum class c {
		bg,
		logo1,
		logo2,
		inside,
		outside,
		outline,
		blank,
		input,
		light1,
		light2,
		dark,
		selection,
		keyC,
		keyG,
		clearWhite,
	};

	// uint8_t range : 0-255
	// usage : theme[c::bg] NOT theme[1]
	// for code readability
	extern unsigned char theme[][3];

	extern spacing cs;
}

#endif