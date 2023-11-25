


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT
#define FONT		UFDIR"76london38.uf"

static const char *strtext = "Hello !dlrow ";


static _ufont_t font;
static _ufont_palette_t *pal;
static _ufont_surface_t *surface;



void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, FONT)){
		//printf("fontOpen() failed\r\n");
		return;
	}
		
	tft_clear(COLOUR_BLACK);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);

	pal = fontPaletteLoad("palettes/pal1.png");

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_24TO16(0xFFBF33), NULL);
	
	// make this the active surface
	fontSetRenderSurface(&font, surface);
}

void drawHellowWorld ()
{
	int x = 10, y = 10;
	
	// draw text on to the active surface
	fontPrint(&font, &x, &y, (uint8_t*)strtext);
		
	x = TFT_WIDTH/2;
	y = TFT_HEIGHT/2;
		
	float incr = 360.0f / (float)(strlen(strtext));
	float length = 100.0f;
	int width;
	int height;
	char text[2] = {0, 0};
		

	tft_clear(COLOUR_BLACK);
	
	for (float a = 90.0f; a < (3.0f*360.0f)+90.0f; a += 0.7f){
		int ct = 0;
		fontCleanSurface(&font, NULL);

		for (float curang = -a; (curang < 360.0f - a) && strtext[ct]; curang += incr){
			float angle = curang * (2.0f * 3.14159265f) / 360.0f;
			float x1 = cosf(angle) * length + x;
			float y1 = sinf(angle) * length + y;

			text[0] = strtext[ct++];
			fontGetMetrics(&font, (uint8_t*)text, &width, &height);

			int _x = x1 - (width/2);
			int _y = y1 - (height/2);
			fontPrint(&font, &_x, &_y, (uint8_t*)text);
		}

		tft_clearBuffer(COLOUR_BLACK);
		fontApplySurfacePalette(&font, pal, 0, 0);
		tft_update();
		//delay(1);
	}
}

void loop ()
{
	drawHellowWorld();
	delay(2000);
}

