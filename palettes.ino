


#include "common.h"

// Requires PSRAM when compiling with FASTEST
// Must set USE_DMAMEM_BUFFER to 1 in common.h

#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


#define FONT		UFDIR"76london38.uf"
static const char *text = "Hello World!";


static _ufont_t font;
static _ufont_surface_t *surface;
static _ufont_palette_t *col256;
static int x = 10;
static int y = 10;



static int HSLtoRGB (float h, float s, float l)
{
	float r, g, b; //this function works with floats between 0 and 1
	float temp1, temp2, tempr, tempg, tempb;
	h = h / 256.0f;
	s = s / 256.0f;
	l = l / 256.0f;
	
	//If saturation is 0, the colour is a shade of grey
	if (s == 0.0f){
		r = g = b = l;

	}else{
		//set the temporary values
		if (l < 0.5f)
			temp2 = l * (1.0f + s);
		else
			temp2 = (l + s) - (l * s);
		temp1 = 2.0f * l - temp2;
 		tempr = h + 1.0f / 3.0f;
		
		if (tempr > 1.0f)
			tempr--;
		tempg = h;
		tempb = h-1.0f / 3.0f;
		
		if (tempb < 0.0f)
			tempb++;
		
		//red
		if (tempr < 1.0f / 6.0f) r = temp1 + (temp2 - temp1) * 6.0f * tempr;
		else if (tempr < 0.5f) r = temp2;
		else if (tempr < 2.0f / 3.0f) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0f) - tempr) * 6.0f;
		else r = temp1;
		
		 //green
		if (tempg < 1.0f / 6.0f) g = temp1 + (temp2 - temp1) * 6.0f * tempg;
		else if (tempg < 0.5f) g = temp2;
		else if (tempg < 2.0f / 3.0f) g = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempg) * 6.0f;
		else g = temp1;
		
		//blue
		if (tempb < 1.0f / 6.0f) b = temp1 + (temp2 - temp1) * 6.0f * tempb;
		else if (tempb < 0.5f) b = temp2;
		else if (tempb < 2.0f / 3.0f) b = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempb) * 6.0f;
		else b = temp1;
	}
	
	uint8_t rr = (int)(r * 255.0f);
	uint8_t gg = (int)(g * 255.0f);
	uint8_t bb = (int)(b * 255.0f);
	
	return (rr << 16)|(gg << 8)|bb;
}

static void RGB24ToRGB16 (uint8_t *pal24, uint16_t *pal16, int tpixels)
{
	
	for (int i = 0; i < tpixels; i++){
		uint32_t col = *(uint32_t*)(&pal24[i*3])&0xFFFFFF;
		pal16[i] = COLOUR_24TO16(col);
	}

/*	
	// and back again..
	int c = tpixels;
	for (int i = tpixels-1; i >= 0; i--){
		uint32_t col = *(uint32_t*)(&pal24[i*3])&0xFFFFFF;
		//printf("%i: %X\n", i, col);
		pal16[c++] = COLOUR_24TO16(col);
	}
*/
}

void genPaletteFire (uint16_t *pal16, const int32_t tColours)
{
	for (int x = tColours-1; x>=0 ; x--){
		//HSLtoRGB is used to generate colors:
		//Hue goes from 0 to 85: red to yellow
		//Saturation is always the maximum: 255
		//Lightness is 0..255 for x=0..128, and 255 for x=128..255
		//set the palette to the calculated RGB value
    	uint32_t c = HSLtoRGB(x / 3.0f, 255.0f, min(255.0f, x * 2.0f));

   		uint32_t r = (c&0xF80000)>>8;
		uint32_t g = (c&0x00FC00)>>5;
		uint32_t b = (c&0x0000F8)>>3;
		*pal16++ = (r|g|b)&0xFFFF;
	}
}


static inline char *advance (char *buffer)
{
	char *ptr = buffer;
	for (int i = 0; i < 32; i++){
		if (*ptr == '\n')
			return ++ptr;
		
		ptr++;
	}
	return NULL;
}

static int importPalAsc (const char *file, uint16_t *pal16, const int32_t tColours)
{
	FsFile *fio = fio_open((uint8_t*)file, FIO_READ);
	if (!fio) return 0;

	uint8_t pal24[tColours*3];
	//uint8_t *pal24 = extmem_malloc(tColours*3);
	uint8_t *buffer = pal24;

	int r, g, b;
	int32_t ct = 0;
	
	int len = fio->size();
	char *_colours = (char*)/*extmem_*/malloc(len+8);
	char *colours = _colours;
	
	len = (int)fio_read(fio, colours, len);
	fio_close(fio);

	int passes = 0;
	while (passes < tColours){
		int ret = sscanf(colours, "%i %i %i\r\n", &r, &g, &b);
		if (ret != 3) break;
			
		
		//printf("%i %i %i\r\n", r, g, b);
		
		buffer[2] = r&0xFF;
		buffer[1] = g&0xFF;
		buffer[0] = b&0xFF;
		buffer += 3;
		ct++;

		colours = advance(colours);	// go to next line
		if (!colours) break;
	};

	//if (ct == tColours)
		RGB24ToRGB16(pal24, pal16, tColours);
		
	//extmem_free(_colours);
	//extmem_free(pal24);
	return ct;
}

static inline int importPalBin (char *file, uint16_t *pal16, const int32_t tColours)
{
	uint8_t pal24[tColours*3];
	//uint8_t *pal24 = /*extmem_*/malloc(tColours*3);
	if (!pal24) return 0;
	
	fileio_t *fio = fio_open((uint8_t*)file, FIO_READ);
	if (fio){
		int ret = 0;
		//if (fio_length == 768)
			ret = fio_read(fio, pal24, sizeof(pal24));
		//else
		//	printf("importPalBin: filesize is not 256x3\n");
			
		fio_close(fio);
		
		if (ret == sizeof(pal24) && (ret == tColours*3)){
			RGB24ToRGB16(pal24, pal16, tColours);
			//extmem_free(pal24);
			return tColours;
		}
	}
	
	//extmem_free(pal24);
	return 0;
}

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

	if (!fontOpen(&font, FONT)){
		//printf("fontOpen() failed \r\n");
		return;
	}
		
	tft_clear(COLOUR_BLACK);

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(TFT_WIDTH, TFT_HEIGHT, COLOUR_24TO16(0xFFBF33), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	fontSetRenderSurface(&font, surface);
	fontSetPaletteAxis(surface, SURFACE_PALETTE_DIR_H);
	
	uint16_t pal16[256] = {
	   	gray8to16(0x33),
	   	gray8to16(0xAA),
	   	gray8to16(0x33),
	   	gray8to16(0xAA),
	   	gray8to16(0x33),
	   	gray8to16(0xAA),
	   	gray8to16(0x33),
	   	gray8to16(0xAA)
	};


	_ufont_palette_t *pal = fontPaletteLoad("palettes/pal3.png");
	fontPrint(&font, &x, &y, text);
	fontApplySurfacePalette(&font, pal, 0, 0);

	fontSetPaletteAxis(surface, SURFACE_PALETTE_DIR_V);
	fontCleanSurface(&font, NULL);
	fontPrint(&font, &x, &y, text);
	fontApplySurfacePalette(&font, pal, 0, 0);
	

	_ufont_palette_t *col16 = fontPaletteCreate(pal16, 8);
	fontCleanSurface(&font, NULL);
	fontPrint(&font, &x, &y, text);
	fontApplySurfacePalette(&font, col16, 0, 0);

	importPalAsc("palettes/Ocean.pal", pal16, 256);
	col256 = fontPaletteCreate(pal16, 256);
	fontCleanSurface(&font, NULL);
	fontPrint(&font, &x, &y, text);
	fontApplySurfacePalette(&font, col256, 0, 0);
	fontSetRenderColour(surface, pal16[20]);
	fontApplySurfaceOutline(&font, 0, 0);


	importPalAsc("palettes/Autumn.pal", pal16, 256);
	fontSetPaletteAxis(surface, SURFACE_PALETTE_DIR_H);
	fontCleanSurface(&font, NULL);
	fontPrint(&font, &x, &y, text);
	fontApplySurfacePalette(&font, col256, 0, 0);
	fontSetRenderColour(surface, pal16[20]);
	fontApplySurfaceOutline(&font, 0, 0);


	fontSetPaletteAxis(surface, SURFACE_PALETTE_DIR_V);
	importPalBin("palettes/palette256.bin", pal16, 256);
	fontCleanSurface(&font, NULL);
	y += 2;
	fontDrawRectangleFilled(&font, 142, y, DWIDTH-10, DHEIGHT-10, 1);
	y += 4;// x += 100;
	fontSetRenderFlags(&font, 0);	// don't advance Y
	fontSetWriteOp(&font, BFONT_PIXELOP_INVERT);
	fontPrint(&font, &x, &y, text);
	fontSetWriteOp(&font, BFONT_PIXELOP_DEFAULT);
}

void loop ()
{
	volatile int32_t len = 75;
	for (int i = 0; i < 256; i++){
		if (i+len >= 256) len = 256-i;

		fontApplySurfacePaletteEx(&font, col256, i, len, 0, 0);
		tft_update();
		delay(5);
	}

	fontCleanSurface(&font, NULL);
	fontPrint(&font, &x, &y, text);
	fontApplySurfaceOutline(&font, 0, 0);
	tft_update();
	
	
	fontCleanSurface(&font, NULL);
	y -= 4;
	fontDrawRectangleFilled(&font, 142, y, DWIDTH-10, DHEIGHT-10, 1);
	y += 4;// x += 100;
	fontSetRenderFlags(&font, 0);	// don't advance Y
	fontSetWriteOp(&font, BFONT_PIXELOP_INVERT);
	fontPrint(&font, &x, &y, text);
	fontSetWriteOp(&font, BFONT_PIXELOP_DEFAULT);	
	
	
	delay(2000);
}

