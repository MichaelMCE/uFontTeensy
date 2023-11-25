


#include "common.h"
#include "libuf/fonts.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


static _ufont_t font;
static _ufont_surface_t *surface;




typedef struct{
	int id;
	int width;
	int height;
	char file[128];
	//char *file;
}uf_met_t;

DMAMEM uf_met_t ufm[(sizeof(fonts)/sizeof(*fonts))];


#define RENDERFLAGS	(BFONT_RENDER_DEFAULT )
//#define RENDERFLAGS	(BFONT_RENDER_DEFAULT | BFONT_RENDER_FIXEDWIDTH)

static const char *text = "The quick brown fox jumps over the lazy dog.0123456789:";


static inline void swapi32 (int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static inline void sortByWidth (uf_met_t *fid, int total)
{
	if (!total) return;
	
	char pathtemp[128];	
	int i = --total;

	while(i--){
		for (int j = 0; j < total; j++){
			if (fid[j].width > fid[j+1].width){
				swapi32(&fid[j].id, &fid[j+1].id);
				swapi32(&fid[j].height, &fid[j+1].height);
				swapi32(&fid[j].width, &fid[j+1].width);
				
				strcpy(pathtemp, fid[j+1].file);
				strcpy(fid[j+1].file, fid[j].file);
				strcpy(fid[j].file, pathtemp);
			}
		}
	}
}

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n\r\nComputing...\r\n");

	sd_init();
	tft_init();

	/*if (!fontOpen(&font, FONTFILE)){
		printf("fontOpen() failed \r\n");
		return;
	}*/
		
	tft_clear(COLOUR_WHITE);
	tft_update();
	tft_update();
	
	// create a 1BPP work surface. this is the initial work surface
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_G4TO16(6), NULL);
	fontSetRenderSurface(&font, surface);



	for (int i = 0; fonts[i]; i++){
		if (fontOpen(&font, fonts[i])){
			fontSetRenderFlags(&font, RENDERFLAGS);
			
			int width = 0;
			int height = 0;
			fontGetMetrics(&font, (uint8_t*)text, &width, &height);

			ufm[i].id = i;
			ufm[i].width = width;
			ufm[i].height = height;
			strcpy(ufm[i].file, fonts[i]);
			//ufm[i].file = strdup(fonts[i]);
			fontClose(&font);
		}else{
			printf("failed to open %i: '%s'\r\n", i, fonts[i]);
		}
	}


	const int total = (sizeof(fonts) / sizeof(*fonts))-1;	
	sortByWidth(ufm, total);

}

void loop ()
{

	static const int total = (sizeof(fonts) / sizeof(*fonts))-1;
	
	int x = 2, y = 1;
	tft_clearBuffer(COLOUR_CREAM);
	
	for (int i = 0; i < total; i++){
		if (!fontOpen(&font, fonts[ufm[i].id])){
			printf("failed to open %i: '%s'\r\n", i, fonts[ufm[i].id]);
			continue;
		}

		fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
		fontSetRenderSurface(&font, surface);
		fontSetRenderFlags(&font, RENDERFLAGS);

		if (y+ufm[i].height >= DHEIGHT){
			y = 1;
			fontCleanSurface(&font, NULL);
			tft_clearBuffer(COLOUR_CREAM);
			delay(2000);
			printf("\r\n");
		}

		printf("font %i %i: \t'%s'\r\n", (font.header.flags&BFONT_FLAGS_MONOSPACE)>0, ufm[i].id, fonts[ufm[i].id]);
		
		if (!fontPrint(&font, &x, &y, (uint8_t*)text))
			printf(" font render failed\n");

		fontApplySurface(&font, 0, 0);
		tft_update();
		fontClose(&font);
	}

	tft_update();
	delay(2000);
}

