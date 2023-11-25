


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


//#define fontFile	UFDIR"unicode36.uf"
//#define fontFile	UFDIR"10x20.uf"
//#define fontFile	UFDIR"utopia_b24.uf"
#define fontFile	UFDIR"lucida18.uf"
//#define fontFile	UFDIR"unifont32.uf"
//#define fontFile	UFDIR"freesans32.uf"

static PROGMEM const uint8_t *str = (const uint8_t*)"This program is free software; you can redistribute it and/or "
	 "modify it under the terms of the GNU LIBRARY GENERAL PUBLIC LICENSE "
	 "as published by the Free Software Foundation; either version 2 "
	 "of the License, or (at your option) any later version.";

static _ufont_t font;
static _ufont_surface_t *surface;


static inline uint16_t gray8to16 (const uint8_t val)
{
	return COLOUR_24TO16(COLOUR_G8TO24(val));
}


void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, fontFile)){
		printf("fontOpen() failed \r\n");
		return;
	}
		
	tft_clear(0xEEEE);

	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	
	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(TFT_WIDTH, TFT_HEIGHT, COLOUR_24TO16(0x777777), NULL);
	fontSetRenderSurface(&font, surface);
	
	// setup without wordwrap
	fontSetRenderFlags(&font, BFONT_RENDER_NEWLINE|BFONT_RENDER_RETURN|BFONT_RENDER_NODEFAULT|BFONT_RENDER_ADVANCE_X|
								BFONT_RENDER_ADVANCE_Y|BFONT_RENDER_BOUNDRECT);


	int width, height;
	fontGetMetrics(&font, (uint8_t*)str, &width, &height);
	//printf("metrics WxH: %i,%i\n", width, height);

	int y = 5;
	int x = 50;
	fontDrawLine(&font, x, 0, x, DHEIGHT-1, 1);
	fontPrint(&font, &x, &y, str);

	int desX = 50;
	x = desX;
	y += 25;
	
	// enable wordwrap, render text with a bounding box rectangle surrounding complete string
	fontSetRenderFlags(&font,BFONT_RENDER_WORDWRAP|BFONT_RENDER_NEWLINE|BFONT_RENDER_NODEFAULT|
		BFONT_RENDER_ADVANCE_X|BFONT_RENDER_ADVANCE_Y|BFONT_RENDER_BOUNDRECT);
	fontPrint(&font, &x, &y, str);

	x = desX;
	y += 40;
	
	// render glyphs with bounding box per glyph
	fontSetRenderFlags(&font,BFONT_RENDER_WORDWRAP|BFONT_RENDER_NEWLINE|BFONT_RENDER_NODEFAULT|
		BFONT_RENDER_ADVANCE_X|BFONT_RENDER_ADVANCE_Y|BFONT_RENDER_GLYPHRECT);
	fontPrint(&font, &x, &y, str);


	// render 1bit buffer to 16bit front buffer
	fontApplySurface(&font, 0, 0);
	tft_update();
}

void loop ()
{
	delay(200);
}

