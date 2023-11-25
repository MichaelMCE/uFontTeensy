


#include "common.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

#define FONT		"uf/ravebold72.uf"

static _ufont_t fontOLED;
static _ufont_surface_t *surfaceOLED;




void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	oled_init();
	
	if (!fontOpen(&fontOLED, FONT)){
		//printf("fontOpen() failed \r\n");
		return;
	}
	surfaceOLED = fontCreateSurface(OLED_WIDTH, OLED_HEIGHT, 1, oled_getBuffer());
	fontSetRenderSurface(&fontOLED, surfaceOLED);
	fontSetRenderFlags(&fontOLED, BFONT_RENDER_ADVANCE_X);

	int x = -8;
	int y = -12;
	
	oled_clear();
	fontPrint(&fontOLED, &x, &y, (uint8_t*)"128");

	oled_update();
}	
	

void loop ()
{

	
	delay(1000);
}

