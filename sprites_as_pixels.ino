


#include "common.h"
#include "libuf/fonts.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


static _ufont_t font;
static _ufont_surface_t *surface;




static inline char *stristr (const char *string, const char *pattern)
{
	if (!string || !pattern)
		return NULL;
	
	char *pptr, *sptr, *start;
	
	for (start = (char*)string; *start != 0; start++){
		for ( ; ((*start != 0) && (toupper(*start) != toupper(*pattern))); start++){
		}
		
		if (!*start) return NULL;

		pptr = (char*)pattern;
		sptr = (char*)start;

		while (toupper(*sptr) == toupper(*pptr)){
			sptr++;
			pptr++;
			if (!*pptr)
				return (start);
		}
	}
	return NULL;
}

static inline int utilFontLookup (const char *name)
{
	int i = 0;
	
	do{
		if (stristr(fonts[i], name))
			return i;
	}while(fonts[++i]);

	return 0;
}

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n\r\nComputing...\r\n");

	sd_init();
	tft_init();
	tft_clear(COLOUR_WHITE);
	tft_update();

	
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_CREAM, NULL);
	if (!surface) return;
	
	_ufont_surface_t *sprite[2];
	sprite[1] = fontCreateTexture("images/spot8.png");
	sprite[0] = fontCreateTexture("images/shadow6.png");


	int x = 4;
	int y = 4;
	
	const int total = sizeof(fonts) / sizeof(*fonts);
	int start = utilFontLookup("london38");
	
	for (int i = start; i < start+3 && y < DHEIGHT-1; i++){
		if (!fontOpen(&font, fonts[i])){
			printf("font open failed for %i: '%s'\r\n", i, fonts[i]);
			continue;
		}
			
		printf("font %i: '%s'\r\n", i, fonts[i]);
		
		fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
		fontSetRenderSurface(&font, surface);
		fontSetLineSpace(&font, 6);
		
		fontPrint(&font, &x, &y, "0123456789:-");

		fontApplySurfaceSprite(&font, sprite[i&0x01], COLOUR_BLACK, 2, 2, 0, 0);
		fontCleanSurface(&font, NULL);
		fontClose(&font);
		
		tft_update();
		//delay(2000);
	}
	
	delay(2000);
	
	x = 4;
	y = 4;
	
	tft_clear(COLOUR_WHITE);
	fontCleanSurface(&font, NULL);
	
	for (int i = start+3; i < start+4 && y < DHEIGHT-1; i++){
		if (!fontOpen(&font, fonts[i])){
			printf("font open failed for %i: '%s'\r\n", i, fonts[i]);
			continue;
		}
			
		printf("font %i: '%s'\r\n", i, fonts[i]);
		
		fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
		fontSetRenderSurface(&font, surface);
		fontSetLineSpace(&font, 6);
		
		fontPrint(&font, &x, &y, "0123456789:-");

		fontApplySurfaceSprite(&font, sprite[i&0x01], COLOUR_BLACK, 2, 2, 0, 0);
		fontCleanSurface(&font, NULL);
		fontClose(&font);
		
		tft_update();
		//delay(2000);
	}
	
	delay(2000);
	tft_clear(COLOUR_WHITE);
	fontCleanSurface(&font, NULL);

	x = 4;
	y = 4;
			
	for (int i = start+4; i < start+5 && y < DHEIGHT-1; i++){
		if (!fontOpen(&font, fonts[i])){
			printf("font open failed for %i: '%s'\r\n", i, fonts[i]);
			continue;
		}
			
		printf("font %i: '%s'\r\n", i, fonts[i]);
		
		fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
		fontSetRenderSurface(&font, surface);
		fontSetLineSpace(&font, 6);
		
		fontPrint(&font, &x, &y, "0123456789:-");

		fontApplySurfaceSprite(&font, sprite[i&0x01], COLOUR_BLACK, 2, 2, 0, 0);
		fontCleanSurface(&font, NULL);
		fontClose(&font);
		
		tft_update();
		//delay(2000);
	}

	fontSurfaceFree(surface);
	fontSurfaceFree(sprite[0]);
	fontSurfaceFree(sprite[1]);
}

void loop ()
{
	tft_update();
	delay(100);
}

