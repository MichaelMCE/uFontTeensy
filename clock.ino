


#include "common.h"
#include <TimeLib.h>

#define SHOW_BACKGROUND	0
#define SHOW_SECONDS	0


#define DWIDTH			TFT_WIDTH
#define DHEIGHT			TFT_HEIGHT
#define COLOUR_OUTLINE	COLOUR_24TO16(0xFEF7F0)



//#define FONTFILE		UFDIR"teutonweiss-bold132.uf"
//#define FONTFILE		UFDIR"rationale-regular190.uf"
#define FONTFILE		UFDIR"ravebold140.uf"


static _ufont_t font;
static _ufont_surface_t *surface;
static _ufont_surface_t *texture;

#if SHOW_BACKGROUND
static _ufont_surface_t *background;
#endif


int getCharWidth (_ufont_t *font, uint8_t ch)
{
	int width = 0;
	uint16_t text[] = {ch, 0};
	fontGetMetricsList(font, text, &width, NULL);
	return width;
}


// find the smallest rect that will fit '88:88:88'
static void calcClockRect (_ufont_t *font, int *width, int *height)
{
	int maxW = 0;
	int maxH = 0;
	
	for (char i = '0'; i <= '9'; i++){
		char text[] = {i, i, ':', i, i, ':', i, i, 0};
		fontGetMetrics(font, (uint8_t*)text, width, height);

		if (*width > maxW) maxW = *width;
		if (*height > maxH) maxH = *height;
	}

	*width = maxW;
	*height = maxH;
}

static time_t getTeensyTime ()
{
	return Teensy3Clock.get();
}

unsigned long processSyncMessage ()
{
	#define TIME_HEADER  "T"   // Header tag for serial time sync message
	
	unsigned long pctime = 0L;
	const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

	if (Serial.find(TIME_HEADER)){
		pctime = Serial.parseInt();
		printf("%i\r\n", (int)pctime);
		return pctime;
		
		if (pctime < DEFAULT_TIME){ // check the value is a valid time (greater than Jan 1 2013)
       		pctime = 0L; // return 0 to indicate that the time is not valid
		}
	}
	
	return pctime;
}

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");
	tft_init();
	sd_init();

	if (!fontOpen(&font, FONTFILE)){
		//printf("fontOpen() failed \r\n");
		return;
	}

	fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
	fontSetGlyphPadding(&font, fontGetGlyphPadding(&font)+1);		

#if SHOW_BACKGROUND
	// background should be sized to cover display
	background = fontCreateTexture("images/sin-jong-hun.png");
#endif
	
	// render time from this texture
	texture = fontCreateTexture("images/carbon2.png");
	
	int clkwidth = 0;
	int clkheight = 0;
	calcClockRect(&font, &clkwidth, &clkheight);
	//printf("::%i %i\r\n", clkwidth, clkheight);

	// create a 1BPP work surface, which is also the initial work surface
	surface = fontCreateSurface(clkwidth, clkheight, COLOUR_OUTLINE, NULL);
	fontSetRenderSurface(&font, surface);
	fontSetRenderFlags(&font, BFONT_RENDER_ADVANCE_X/*|BFONT_RENDER_BOUNDRECT| BFONT_RENDER_GLYPHRECT*/ | 
		(BFONT_RENDER_CONDENSED*(SHOW_SECONDS&0x01)));

	setSyncProvider(getTeensyTime);
#if 0
	if (timeStatus() != timeSet){
		printf("Unable to sync with the RTC\r\n");
	}else{
		printf("RTC has set the system time\r\n");
	}
#endif
		
	time_t t = processSyncMessage();
	if (t != 0){
		Teensy3Clock.set(t); // set the RTC
		setTime(t);
	}
}

void loop ()
{

	static char timestr[12];

	fontCleanSurface(&font, NULL);
#if SHOW_SECONDS
	sprintf(timestr, "%.2i:%.2i:%.2i", hour(), minute(), second());
#else
	sprintf(timestr, "%.2i:%.2i", hour(), minute());
#endif
	
	int width = 0;
	int height = 0;
	fontGetMetrics(&font, (uint8_t*)timestr, &width, &height);
		
	int x = (abs(surface->width - width)/2) - 1;
	//int y = abs(surface->height - height)/2;
	int y = -(font.header.fontDescent/2);
	fontPrint(&font, &x, &y, (uint8_t*)timestr);
		
	x = (DWIDTH - surface->width)/2;
	y = (DHEIGHT - surface->height)/2;

#if SHOW_BACKGROUND
	memcpy(tft_getBuffer(), fontSurfaceGetPixels(background), fontSurfaceGetSize(background));
#else
	tft_clearBuffer(COLOUR_BLACK);
#endif
	fontApplySurfaceTexture(&font, texture, x, y);
	fontApplySurfaceOutline(&font, x, y);
	tft_update();

	delay(200);
	
	time_t t = processSyncMessage();
	if (t != 0){
		Teensy3Clock.set(t); // set the RTC
		setTime(t);
	}
}
