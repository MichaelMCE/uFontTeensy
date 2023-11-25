


#include "common.h"
#include "libuf/fonts.h"




#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT


static _ufont_t font;
static _ufont_surface_t *surface;



#define fontFile	UFDIR"10x20.uf"




void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n\r\nComputing...\r\n");

	sd_init();
	tft_init();
	tft_clear(COLOUR_WHITE);
	tft_update();
	
	if (!fontOpen(&font, fontFile)){
		printf("fontOpen() failed \r\n");
		return;
	}


	// create a 1BPP work surface. this is the initial work surface
	fontSetDisplayBuffer(&font, tft_getBuffer(), DWIDTH, DHEIGHT);
	surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_G4TO16(6), NULL);
	fontSetRenderSurface(&font, surface);
	fontCleanSurface(&font, NULL);


	fontSetRenderColour(surface, COLOUR_24TO16(0x777777));
	fontSetGlyphPadding(&font, 0);
	fontSetLineSpace(&font, 2);
	uint32_t flags = BFONT_RENDER_ADVANCE_Y | BFONT_RENDER_FIXEDWIDTH | BFONT_RENDER_COMBINED | BFONT_RENDER_RETURN | BFONT_RENDER_NEWLINE;
	fontSetRenderFlags(&font, flags);

}

void loop ()
{
	tft_clearBuffer(COLOUR_WHITE);
	fontCleanSurface(&font, NULL);
	
	int x = 10; int y = 2;
	fontPrint8(&font, &x, &y, 
		"2380 20E3 ⎀⃣ < Keyboard Symbols with\r\n"
		"2381 20E3 ⎁⃣    combining enclosing keycap\r\n"
		"2382 20E3 ⎂⃣ \r\n"
		"2383 20E3 ⎃⃣ \r\n"
		"2384 20E3 ⎄⃣ \r\n"
		"2385 20E3 ⎅⃣ \r\n"
		"2386 20E3 ⎆⃣ \r\n"
		"2387 20E3 ⎇⃣ \r\n"
		"2388 20E3 ⎈⃣ \r\n"
		"238C 20E3 ⎌⃣ \r\n"
		"2139 20DD ℹ⃝ < Info source with combining\r\n"
		"2397 20E3 ⎗⃣   enclosing circle\r\n"
		"2398 20E3 ⎘⃣ \r\n"
		"2399 20E3 ⎙⃣ \r\n"
		"239A 20E3 ⎚⃣ \r\n"
	);

	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(4000);
	fontCleanSurface(&font, NULL);
	
	
	y = 1;
	fontPrint8(&font, &x, &y, "b̸̛̠͕͉̘͇̻̪̺̰̾̽̓̃͒̊̍͂ͯ̊̐ͬ͟͡͞ͅͅͅe̵̶̸̗͕͈͚̹̻̝͇̲͎̗͑ͦ̈́̋̎̀̐̋̎̎͐͋͂̔͐͋̂ͅḩ̴̨̛̫̻̖̙͚̻̣͉̺̻̭͚͎̻̺̮͙͚ͨͩ͐̾ͫ̿̔̉̏̃̾̊ͬ͐̍ͩ̑̓ͮi̧̼̝̫̭͉͗͑ͫ̈̿̽̍̓͑͛͋ͣͦ̋̑͆̈́ͅn̷̛͓͙͉̪͖̯͈͙̹̏̈̅̓ͮ̀ͅd̴̷͍̼̹͙̥̙̬͚̫̙̥̭̱̜̗̀̔̆͂̓ͥͫ̈͜ ̴̨̪̰̠̮̼͉͕͓̗͑̌ͨ̈́̿ͤ̆͋ͯ͗͆̕ť̵̷̡̛͇̜̜̎͑̌ͬͮ̄̊ͪ͆ͨ̀ͅh̨͔̠͇͖͕͚̣̫̄ͬ̄̾ͤ̑̑͌ͪͣ̍͌̌ͦ̅̆͌̚͟͢͝e̊̈́ͪ͐̌̀͝҉̢̣͎̞̱͙̪̗ ̔̂̊͌̔͗ͫͪ̋̊̇̍҉͏͇̝̭̝͎̞͚̞̳͕͔̯̪͎̫̲̗̯ͅw̷̡̢̠̘̠̙̻̳̞͐ͥͬ̉̿ͤ̇͌͋̿̚a̶͛̉̆́҉̖͇̱͚͙̭̟͓͙̫̘̟͈̫͟l̛̙̳̤̔̓̏͋͆ͮͩ̀ͥ̏ͮ̋͂ͩ̔̌́͘͞l͋ͧͮ̅ͯͭ̔ͯ͆ͩͦ̍͏̷̶̡͇̤̯̖̪̹͓̖́");
	y += 10;
	fontPrint8(&font, &x, &y, "The Greek word 'kosme': \"κόσμε\"");
	fontPrint8(&font, &x, &y, "STARGΛ̊TE SG-1, a = v̇ = r̈, a⃑ ⊥ b⃑");


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(4000);
	fontCleanSurface(&font, NULL);


	x = 1; y = 1;
	fontPrint8(&font, &x, &y, 
		"  [----------------------------|------------------------]\r\n"
		"    ๏ แผ่นดินฮั่นเสื่อมโทรมแสนสังเวช  พระปกเกศกองบู๊กู้ขึ้นใหม่\r\n"
		"  สิบสองกษัตริย์ก่อนหน้าแลถัดไป       สององค์ไซร้โง่เขลาเบาปัญญา\r\n"
		"    ทรงนับถือขันทีเป็นที่พึ่ง           บ้านเมืองจึงวิปริตเป็นนักหนา\r\n"
		"  โฮจิ๋นเรียกทัพทั่วหัวเมืองมา         หมายจะฆ่ามดชั่วตัวสำคัญ\r\n"
		"    เหมือนขับไสไล่เสือจากเคหา      รับหมาป่าเข้ามาเลยอาสัญ\r\n"
		"  ฝ่ายอ้องอุ้นยุแยกให้แตกกัน          ใช้สาวนั้นเป็นชนวนชื่นชวนใจ\r\n"
		"    พลันลิฉุยกุยกีกลับก่อเหตุ          ช่างอาเพศจริงหนาฟ้าร้องไห้\r\n"
		"  ต้องรบราฆ่าฟันจนบรรลัย           ฤๅหาใครค้ำชูกู้บรรลังก์ ฯ"
		);

	y += 8;
	
	
	
	uint32_t flags = fontGetRenderFlags(&font);
	fontSetRenderFlags(&font, BFONT_RENDER_WORDWRAP);
		
	fontPrint8(&font, &x, &y, 
		"The above is a two-column text. If combining characters are handled correctly,"
		" the lines of the second column should be aligned with the | character above.");

	fontSetRenderFlags(&font, flags);

	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(4000);
	fontCleanSurface(&font, NULL);


}

