


#include "common.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

static _ufont_t font;




#define ANIMATION_SPEED	(50.0f)
#define BALLWIDTH		(7.33f)
#define BOUNCE_WIDTH    (DWIDTH-2)
#define BOUNCE_HEIGHT   (DHEIGHT-2)
#define WALL_L_OFFSET   -BALLWIDTH
#define WALL_R_OFFSET   -BALLWIDTH

static float ball_x = 0.0f;
static float ball_y = 0.0f;



static inline float sin_deg (const float deg)
{
	return sinf(DEG2RAD * deg);
}

static inline void bounceBall (const float dt)
{
	static float ball_x_inc = 1.0f;
	static float ball_y_inc = 2.0f;
	
	
	/* Bounce on walls */
	if (ball_x >  (BOUNCE_WIDTH/2 + WALL_R_OFFSET))
		ball_x_inc = -0.5f - 0.75f * (float)rand() / (float)RAND_MAX;
   
	if (ball_x < -(BOUNCE_WIDTH/2 + WALL_L_OFFSET))
		ball_x_inc =  0.5f + 0.75f * (float)rand() / (float)RAND_MAX;

	/* Bounce on floor / roof */
	if (ball_y > (BOUNCE_HEIGHT/2.0f)-BALLWIDTH)
		ball_y_inc = -0.75f - 1.0f * (float)rand() / (float)RAND_MAX;

	if (ball_y < -(BOUNCE_HEIGHT/2.0f)+BALLWIDTH)
		ball_y_inc =  0.75f + 1.0f * (float)rand() / (float)RAND_MAX;

	/* Update ball position */
	ball_x += ball_x_inc * (dt*ANIMATION_SPEED);
	ball_y += ball_y_inc * (dt*ANIMATION_SPEED);

	
	// Simulate the effects of gravity on Y movement.
	
	float deg = (ball_y + BOUNCE_HEIGHT/2.0f) * 90.0f / BOUNCE_HEIGHT;
	if (deg > 80.0f) deg = 80.0f;
	if (deg < 10.0f) deg = 10.0f;

	float sign = 1.6f;   
	if (ball_y_inc < 0.0f)
		sign = -0.7f;
	//else
	//	sign = 1.0f;

	ball_y_inc = (sign * 4.0f * sin_deg(deg));
}

static inline void drawFrame ()
{
	fontDrawRectangle(&font, 0, 0, DWIDTH-1, DHEIGHT-1, 1);
	fontDrawRectangle(&font, 1, 1, DWIDTH-2, DHEIGHT-2, 1);
}

static inline void drawBall (int x, int y)
{
	fontDrawCircleFilled(&font, x, y, BALLWIDTH, 1);
}

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, NULL)){
		printf("fontOpen() failed \r\n");
		return;
	}
		
	tft_clear(COLOUR_WHITE);
	tft_update();
	
	// create a 1BPP work surface. this is the initial work surface
	_ufont_surface_t *surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_24TO16(0xFF8F33), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	fontSetRenderSurface(&font, surface);
	randomSeed(analogRead(A9));


}

void loop ()
{



	double t0, t_old = 0.0;
	ball_x = (int)(random(DWIDTH-1))-(DWIDTH/2);
	ball_y = (int)(random(DHEIGHT-1))-(DHEIGHT/2);

	_ufont_palette_t *pal = fontPaletteLoad("palettes/pal3.png");

	do{
		//tft_clear(COLOUR_WHITE);
		//fontCleanSurface(NULL, surface);
		
		drawFrame();
		drawBall(ball_x+(DWIDTH>>1), ball_y+(DHEIGHT>>1));
		fontApplySurfacePalette(&font, pal, 0, 0);
		//fontApplySurfaceOutline(&font, 0, 0);
		
		tft_update();

		t0 = millis()/1000.0f;
		bounceBall(t0 - t_old);
		t_old = t0;
		
		delay(1);
	}while(1);
	
	
	delay(200);
}

