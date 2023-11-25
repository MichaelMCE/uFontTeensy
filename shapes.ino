


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

static _ufont_t font;
static _ufont_surface_t *surface;



static void drawShapes (_ufont_t *font)
{
	_ufont_surface_t *front = fontGetDisplayBuffer(font);
	
	// draw directly to distination surface
	surfaceDrawRectangleFilled(front, front->width*0.4f, 0, front->width-1, front->height/2, COLOUR_CREAM);

	fontInvertRectangle(font, 3, 2, 400, 150);
	
	fontDrawRectangleFilled(font, 50, 50, 200, 200, 0);
	fontDrawRectangle(font, 50, 250, 300, 310, 1);
	fontDrawRectangle(font, 51, 251, 301, 311, 1);
	fontDrawRectangle(font, 52, 252, 302, 312, 1);

	fontDrawLine(font, 52, 25, 310, 310, 1);
	fontDrawCircleFilled(font, 290, 270, 66, 1);
	fontDrawCircleFilled(font, 290, 270, 55, 0);
	fontDrawCircle(font, 300, 200, 66, 1);
	fontDrawCircle(font, 320, 240, 66, 0);			
						
	fontDrawArc(font, 350, 250, -166.6f, 83.0f, 130.3f, 33.0f, 1);
	fontDrawEllipse(font, 500, 150, 156.6f, 33.0f, 1);
	fontInvertRectangle(font, 175, 100, 300, 230);
}


void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, NULL)){
		//printf("fontOpen() failed\r\n");
		return;
	}
		
	tft_clear(COLOUR_WHITE);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_24TO16(0xFFBF33), NULL);
	
	// make this the active surface
	fontSetRenderSurface(&font, surface);
}

void loop ()
{
	fontSetRenderColour(surface, COLOUR_24TO16(0xFFBF33));
	drawShapes(&font);

	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);


	fontSetRenderColour(surface, COLOUR_24TO16(0x779777));
	fontApplySurfaceOutline(&font, 0, 0);
	tft_update();
	delay(2000);
}

