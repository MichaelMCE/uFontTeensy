


#include "common.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT




#define MAXSTARS	512
#define LAYERS		7
#define SPEED		0.30f


typedef struct {
	int32_t x;
	int32_t y;
	uint8_t plane;
}TStar;


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

	randomSeed(analogRead(A9));
	tft_init();
	sd_init();


	if (!fontOpen(&font, NULL)){
		printf("fontOpen() failed \r\n");
		return;
	}

	tft_clear(COLOUR_BLACK);

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(TFT_WIDTH, TFT_HEIGHT, COLOUR_24TO16(0xFFBF33), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	fontSetRenderSurface(&font, surface);


	uint16_t colour[7] = {
	   	gray8to16(0x33),
	   	gray8to16(0x44),
	   	gray8to16(0x55),
	   	gray8to16(0x76),
	   	gray8to16(0x97),
	   	gray8to16(0xAA),
	   	gray8to16(0xFF)
	};
	
    TStar *stars = (TStar*)calloc(sizeof(TStar), MAXSTARS);


    for (int i = 0; i < MAXSTARS; i++){
        stars[i].x = random(DWIDTH<<2);	// no need for the shifts when using float
        stars[i].y = random(DHEIGHT<<2);
        stars[i].plane = random(LAYERS);     // star colour [idx] between 0 and LAYERS
    }

    _ufont_surface_t *surface[LAYERS+1];
    for (int i = 0; i < LAYERS; i++){
    	uint16_t col = (int)(((7.0f/(float)LAYERS)*(float)i));
		surface[i] = fontCreateSurface(DWIDTH, DHEIGHT, colour[col%7], NULL);
	}

	_ufont_surface_t *sprite[LAYERS+1];
	memset(sprite, 0, sizeof(sprite));
	sprite[LAYERS-2] = fontCreateTexture("images/spot8a.png");
	sprite[LAYERS-1] = fontCreateTexture("images/spot8b.png");

	const int pitch = CALC_PITCH_1(DWIDTH);	

	
	while (1){
		for (int i = 0; i < MAXSTARS; i++){
			stars[i].x += (((1.0f+(float)stars[i].plane*4.0f))*SPEED)*1.0f;
			if (stars[i].x >= (DWIDTH<<2)){
				stars[i].x = 0;
				stars[i].y = rand() % (DHEIGHT<<2);
				//stars[i].plane = rand() % LAYERS;
			}
			setPixel1_nb(surface[stars[i].plane], pitch, stars[i].x>>2, stars[i].y>>2);
		}
		
		tft_clearBuffer(COLOUR_BLACK);
		for (int i = 0; i < LAYERS; i++){
			fontSetRenderSurface(&font, surface[i]);
			if (i >= LAYERS-2)
				fontApplySurfaceSprite(&font, sprite[i], 0, 1, 1, 0, 0);
			else
				fontApplySurface(&font, 0, 0);

			fontCleanSurface(NULL, surface[i]);
		}

		//delay(10);
		arm_dcache_flush(tft_getBuffer(), TFT_WIDTH*TFT_HEIGHT*2);
		tft_update();
	}
}

void loop ()
{

	
	delay(2000);
}

