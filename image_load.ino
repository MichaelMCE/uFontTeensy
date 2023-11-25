


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT




static const char *imagePath = "images/sin-jong-hun.png";




void setup ()
{
	tft_init();
	tft_clear(COLOUR_BLACK);
		
#if 0
	Serial.begin(9600);
	while (!Serial);
	printf("uFont\r\n");
#endif

	sd_init();

	_ufont_t font;
	if (!fontOpen(&font, NULL)){
		//printf("fontOpen() failed\r\n");
		return;
	}
	
	fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);

	// loads an image directly on to display buffer (front buffer)
	fontLoadTexture(fontGetDisplaySurface(&font), 0, 0, imagePath);

	// send to display
	tft_update();
			
	uint32_t width = 0;
	uint32_t height = 0;
	
	
	int ret = fontGetTextureMetrics(&width, &height, NULL, imagePath);
	printf("image width:%i, height:%i\r\n", ret, (int)width, (int)height);
	
	if (ret){
		uint8_t *imageArray = (uint8_t*)extmem_calloc(height, CALC_PITCH(width)*2);
		if (imageArray){
			printf("gen tex\r\n");
			_ufont_surface_t *image = fontGenerateTexture(width, height, imageArray);

			printf("load tex\r\n");
			fontLoadTexture(image, 0, 0, imagePath);
			
			printf("surfaceDrawImage tex to bottom right\r\n");
			surfaceDrawImage(fontGetDisplaySurface(&font), image, 0, DWIDTH-width-1, DHEIGHT-height-1);

			printf("surfaceDrawImage update tex\r\n");
			tft_update();

			printf("surfaceBlitImage tex\r\n");
			surfaceBlitImage(fontGetDisplaySurface(&font), image, 30, DHEIGHT-height-20);
			
			printf("surfaceBlitImage update\r\n");
			tft_update();
		
			
			printf("free tex\r\n");
			fontTextureFree(image);
			extmem_free(imageArray);
			
			printf("done\r\n");
		}
	}
}

void loop ()
{
	delay(2000);
}

