


#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


#define TOTAL_NODES		64

typedef struct{
	int dx;
	int dy;
	int px;
	int py;
}tlink_t;


static tlink_t nodes[TOTAL_NODES];
static _ufont_t font;



void mystifyDrawLineConnects (tlink_t *nodes, const int tnodes)
{
	for (int i = 0; i < tnodes-1; i++){
		fontDrawLine(&font, nodes[i].px, nodes[i].py, nodes[i+1].px, nodes[i+1].py, 1);
		fontDrawCircleFilled(&font, nodes[i].px, nodes[i].py, 3, 1);
	}
	
	fontDrawLine(&font, nodes[tnodes-1].px, nodes[tnodes-1].py, nodes[0].px, nodes[0].py, 1);
	fontDrawCircleFilled(&font, nodes[tnodes-1].px, nodes[tnodes-1].py, 3, 1);
}

//update all the nodes coords. adjust each node's speed
//important, dx and dy not =0 or u get strange things happen
void mystifyUpdateNodes (tlink_t *nodes, const int tnodes)
{
	for (int i = 0; i < tnodes; i++){
		nodes[i].px += nodes[i].dx;
		nodes[i].py += nodes[i].dy;
			  
		if (nodes[i].px < 0){
			nodes[i].px = 0;
			nodes[i].dx = 1+(rand()&1);		   //different speed, very important, so u dont get same shape
			
		}else if (nodes[i].px >= DWIDTH-1){
			nodes[i].px = DWIDTH-1;
			nodes[i].dx = -1*(1+(rand()&1));	   //not so sure but works, yup this should be invert
		}
			  
		if (nodes[i].py < 0){
			nodes[i].py = 0;
			nodes[i].dy = 1+(rand()&1);
			
		}else if (nodes[i].py > DHEIGHT-1){
			nodes[i].py = DHEIGHT-1;
			nodes[i].dy = -1*(1+(rand()&1));
		}
	}
}

void mystifyInit (tlink_t *nodes, const int tnodes)
{
	for (int i = 0; i < tnodes; i++){
		nodes[i].px = rand() % DWIDTH;
		nodes[i].py = rand() % DHEIGHT;
		nodes[i].dx = 1;
		nodes[i].dy = 1;
	}
	
	mystifyUpdateNodes(nodes, tnodes);
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
		
	tft_clear(0xEEEE);
	tft_update();
	
	// create a 1BPP work surface. this is the initial work surface
	_ufont_surface_t *surface = fontCreateSurface(TFT_WIDTH, TFT_HEIGHT, COLOUR_24TO16(0x777777), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	fontSetRenderSurface(&font, surface);


	mystifyInit(nodes, TOTAL_NODES);
	
	//_ufont_surface_t *sprite = fontCreateTexture("images/bs.png");

	do{
		fontCleanSurface(NULL, surface);
		mystifyDrawLineConnects(nodes, TOTAL_NODES);
		mystifyUpdateNodes(nodes, TOTAL_NODES);

		tft_clearBuffer(COLOUR_WHITE);
		//fontApplySurfaceSprite(&font, sprite, COLOUR_BLACK, 1, 1, 0, 0);
		fontApplySurfaceOutlineEx(&font, 0, 0, 2);

		//arm_dcache_flush(tft_getBuffer(), TFT_WIDTH*TFT_HEIGHT*2);
		tft_update();
		//delay(10);
	}while(1);


	
}

void loop ()
{

	
	delay(200);
}

