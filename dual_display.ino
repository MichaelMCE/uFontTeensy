


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

#define FONT		"uf/ravebold72.uf"

static _ufont_t font;
static _ufont_surface_t *surface;
static _ufont_t fontOLED;
static _ufont_surface_t *surfaceOLED;


#ifndef DEGTORAD
#define DEGTORAD 0.034906585039f
#endif


typedef struct{
	float x[8];
	float y[8];
	float z[8];
}tcube_t;


static tcube_t cube = {
	{1,  1, -1, -1, -1,  1,  1, -1},
	{1,  1,  1,  1, -1, -1, -1, -1},
	{1, -1, -1,  1,  1,  1, -1, -1}
};

static tcube_t cube2;
static float flength = 320.0f;
static float camz = -4.5f;
static int destx;
static int desty;




static inline void point3DTo2D (const float x, const float y, const float z, const float flength, const float camz, const int x0, const int y0, int *screenx, int *screeny)
{
	if (z-camz < 0.0f) return;
	const float tmp = flength/(z-camz);
	*screenx = (x*tmp)+x0;
	*screeny = (y*tmp)+y0;
}


static inline void rotateX (const float angle, const float y, const float z, float *yr, float *zr)
{
	*yr = y*cosf(angle) - z*sinf(angle);
	*zr = y*sinf(angle) + z*cosf(angle);
}

static inline void rotateY (const float angle, const float x, const float z, float *xr, float *zr)
{
	*xr =  x*cosf(angle) + z*sinf(angle);
	*zr = -x*sinf(angle) + z*cosf(angle);
}

static inline void rotateZ (const float angle, const float x, const float y, float *xr, float *yr)
{
	*xr = x*cosf(angle) - y*sinf(angle);
	*yr = x*sinf(angle) + y*cosf(angle);
}

static inline void drawCube (tcube_t *cube)
{
	int sx = 0;
	int sy = 0;
	int sx2 = 0;
	int sy2 = 0;

	for (int i = 0; i < 7; i++){
		point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
		point3DTo2D(cube->x[i+1],-cube->y[i+1],cube->z[i+1],flength,camz,destx,desty,&sx2,&sy2);
		fontDrawLine(&font, sx, sy, sx2, sy2,  1);
	}
	
	int i = 7;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
	i = 2;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx2,&sy2);
	fontDrawLine(&font,sx,sy,sx2,sy2,  1);


	i = 0;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
	i = 5;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx2,&sy2);
	fontDrawLine(&font,sx,sy,sx2,sy2, 1);	
	
	i = 1;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
	i = 6;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx2,&sy2);
	fontDrawLine(&font,sx,sy,sx2,sy2, 1);

	i = 0;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
	i = 3;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx2,&sy2);
	fontDrawLine(&font,sx,sy,sx2,sy2, 1);
	
	i = 4;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx,&sy);
	i = 7;
	point3DTo2D(cube->x[i],-cube->y[i],cube->z[i],flength,camz,destx,desty,&sx2,&sy2);
	fontDrawLine(&font, sx, sy, sx2, sy2, 1);
}

static inline void rotateCube (tcube_t *in, tcube_t *out, float anglex, float angley, float anglez)
{
	anglex *= DEGTORAD;
	angley *= DEGTORAD;
	anglez *= DEGTORAD;
	memcpy(out, in, sizeof(tcube_t));
	
	for (int i = 0; i < 8; i++){
		rotateX(anglex, out->y[i], out->z[i], &out->y[i], &out->z[i]);
		rotateY(angley, out->x[i], out->z[i], &out->x[i], &out->z[i]);
		rotateZ(anglez, out->x[i], out->y[i], &out->x[i], &out->y[i]);
	}
}

void setup ()
{
	Serial.begin(9600);
	//printf("uFont\r\n");

	sd_init();
	tft_init();
	oled_init();

	if (!fontOpen(&font, NULL)){
		//printf("fontOpen() failed \r\n");
		return;
	}

	// create a 1BPP work surface. this is the initial work surface
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_24TO16(0xFFBF33), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
	fontSetRenderSurface(&font, surface);

	if (!fontOpen(&fontOLED, FONT)){
		//printf("fontOpen() failed \r\n");
		return;
	}
	surfaceOLED = fontCreateSurface(OLED_WIDTH, OLED_HEIGHT, 1, oled_getBuffer());
	fontSetRenderSurface(&fontOLED, surfaceOLED);
	fontSetRenderFlags(&fontOLED, BFONT_RENDER_ADVANCE_X);
	

	flength = (float)(DWIDTH) * 0.83f;
	destx = (float)(DWIDTH) / 2.0f;
	desty = (float)(DHEIGHT) / 2.0f;

}

void drawFPS (const float fps)
{
	static char oled_str[8];
		
	sprintf(oled_str, "%i", (int)fps);

	int oled_x = 0;
	int oled_y = -12;
	int swidth = 0;
	fontGetMetrics(&fontOLED, (uint8_t*)oled_str, &swidth, NULL);

	if (swidth >= OLED_WIDTH)
		oled_x = -8;
	else
		oled_x = (OLED_WIDTH - swidth) / 2;

	fontPrint(&fontOLED, &oled_x, &oled_y, (uint8_t*)oled_str);
}

void loop ()
{
	const float step = 0.43f;
	
	float x0 = 99.0f, y0 = 100.0f, z0 = 99.0f;
 	uint32_t tframes = 0;
	uint32_t start = millis();

	for (float a = 0.0f; a < 3600.0f; a+=step){
		fontCleanSurface(NULL, surface);

		rotateCube(&cube, &cube2, x0, y0+a, z0);
		drawCube(&cube2);

		tft_clearBuffer(COLOUR_WHITE);
		fontSetRenderColour(surface, COLOUR_24TO16(0x775535));
		fontApplySurface(&font, 0, 0);
		fontSetRenderColour(surface, COLOUR_RED);
		fontApplySurfaceOutlineEx(&font, 0, 0, 15);

		tft_update();
		tframes++;
		
		if (!(tframes%70)){
			uint32_t end = millis();
			float time = (float)(end-start)*0.001f;
			float fps = tframes/time;
		
			oled_clear();
			drawFPS(fps);
			oled_update();
		}
	}

	uint32_t end = millis();
	float time = (float)(end-start)*0.001f;
	printf("frames:%d\r\ntime:%.2fs\r\nfps:%.1f\r\n\r\n", (int)tframes, time, tframes/time);
	
	delay(1000);
}

