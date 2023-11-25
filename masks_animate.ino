


#include "common.h"
#include "images/abstract128x128.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


#define QUICKFONT	UFDIR"riskbold38.uf"
#define DIGITFONT	UFDIR"11s0blt.uf"
#define UNIFONT		"uf/unifont32.uf"	// for the CJK glyphs



static _ufont_t font;
static _ufont_t font2;
static _ufont_surface_t *surface;
static _ufont_surface_t *texture;



void setup ()
{
	//Serial.begin(9600);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, DIGITFONT)){
		printf("fontOpen() failed for '%s'\n", DIGITFONT);
		return;
	}

	tft_clear(COLOUR_BLACK);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_BLACK, NULL);

	texture = fontGenerateTexture(128, 128, abstract128x128);
	fontTextureSetClamp(texture, SURFACE_TEXCLAMP_H|SURFACE_TEXCLAMP_V);

	// make this the active surface
	fontSetRenderSurface(&font, surface);

	fontCleanSurface(&font, NULL);
	fontSetRenderColour(surface, COLOUR_24TO16(0x777777));

	int x = 0;
	int y = TFT_HEIGHT/2;

	
	fontDrawRectangleFilled(&font, 200, 50, 466, 200, 1);
	fontDrawCircleFilled(&font, 200, 150, 36, 0);
	
	fontSetRenderSurface(&font, surface);
	fontSetRenderColour(surface, COLOUR_24TO16(0x777777));
	fontSetGlyphPadding(&font, fontGetGlyphPadding(&font)-6);
	fontPrint(&font, &x, &y, (uint8_t*)"0123456789");

	
	x = 5; y = 5;
	
	if (fontOpen(&font2, QUICKFONT)){
		fontSetRenderFlags(&font2, BFONT_RENDER_NEWLINE | BFONT_RENDER_RETURN);
		fontSetRenderSurface(&font2, surface);
		fontSetGlyphPadding(&font2, fontGetGlyphPadding(&font2)+1);
		fontPrint(&font2, &x, &y, (uint8_t*)"The\r\nQuick\r\nbrown fox\r\njumps\r\nover the lazy\r\ndog.");
		fontClose(&font2);
	}

	if (fontOpen(&font2, UNIFONT)){
		fontSetRenderFlags(&font2, BFONT_RENDER_ADVANCE_X);
		fontSetRenderSurface(&font2, surface);
		fontSetGlyphPadding(&font2, fontGetGlyphPadding(&font2)+2);
		
		x = 5; y = DHEIGHT-80;
		const wchar_t *wideHex = L"\u4E00\u266a\u266b\u266C\u266d\u266f \u20AC\u2126\u2122\u307E\uB7C7\u9f98\u7360";
		const char *utf8Hex = " \xe9\xbe\x98";
		const uint16_t alist[] = {32, 9664, 9654, 12540, 51217, 54784, 50556, 54620, 45796, 45800, 48177, 51656, 51060, 46976, 0};
		const char *refs8 = "&#19968;&#36215;&#25169;&#26388;&#36855;&#31163;&#30340;&#32843;&#26657;&#23398;&#29983;&#36830;&#32493;&#22833;&#36394;&#20107;&#20214; ";
		const char *utf8Oct = " \346\227\245\346\234\254\350\252\236";
		
		fontPrintW(&font2, &x, &y, wideHex);
		fontPrint8(&font2, &x, &y, (uint8_t*)utf8Hex);
		fontPrintList(&font2, &x, &y, alist);
		x = 5; y = DHEIGHT-40;
		fontPrint(&font2, &x, &y, (uint8_t*)refs8);
		fontPrint8(&font2, &x, &y, (uint8_t*)utf8Oct);
		
		fontClose(&font2);
	}

	fontApplySurfaceOutline(&font, 0, 0);
}

void loop ()
{
	uint32_t u0 = 0;
	uint32_t u1 = 0;
	
	for (int e = 0; e < 12; e++){
		const uint32_t t0 = millis();
		
		for (int i = 0; i < 100; i++){
			fontApplySurfaceTextureEx(&font, texture, 0, 0, e, millis()-t0);
			
			if (i == 99) u0 = millis();
			tft_update();
			if (i == 99) u1 = millis();
		}

		uint32_t t1 = millis();
		printf("%i: time: %.2f, %i\r\n", e, (float)(t1-t0)/100.0f, (int)(u1-u0));
	}

	delay(2000);
}

