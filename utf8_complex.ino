


#include "common.h"
#include "libuf/fonts.h"


// Arduino IDE does not correctly render UTF8 strings
// Do not adjust the below text formating - uFont will correctly render strings as intended



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
	fontSetLineSpace(&font, 1);
	fontSetRenderFlags(&font, fontGetRenderFlags(&font) | BFONT_RENDER_FIXEDWIDTH);

}

void loop ()
{
	tft_clearBuffer(COLOUR_WHITE);
	fontCleanSurface(&font, NULL);
	int x = 4;
	int y = 4;
	

#if 1
 	fontPrint8(&font, &x, &y, " ╔════════════════╗");
	fontPrint8(&font, &x, &y, "8║░♜▓♞░♝▓♛░♚▓♝░♞▓♜║");
	fontPrint8(&font, &x, &y, "7║▓♟░♟▓♟░♟▓♟░♟▓♟░♟║");
	fontPrint8(&font, &x, &y, "6║░░▓▓░░▓▓░░▓▓░░▓▓║");
	fontPrint8(&font, &x, &y, "5║▓▓░░▓▓░░▓▓░░▓▓░░║");
	fontPrint8(&font, &x, &y, "4║░░▓▓░░▓▓░░▓▓░░▓▓║");
	fontPrint8(&font, &x, &y, "3║▓▓░░▓▓░░▓▓░░▓▓░░║");
	fontPrint8(&font, &x, &y, "2║░♙▓♙░♙▓♙░♙▓♙░♙▓♙║");
	fontPrint8(&font, &x, &y, "1║▓♖░♘▓♗░♕▓♔░♗▓♘░♖║");
	fontPrint8(&font, &x, &y, " ╚════════════════╝");
	fontPrint8(&font, &x, &y, "   a b c d e f g h");
#endif


#if 1
	x = 300;
	y = 30;
	fontPrint8(&font, &x, &y, "⎧⎡⎛┌─────┐⎞⎤⎫");
	fontPrint8(&font, &x, &y, "⎪⎢⎜│a²+b³ ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎪⎢⎜│───── ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎪⎢⎜⎷ c₈   ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎨⎢⎜       ⎟⎥⎬");
	fontPrint8(&font, &x, &y, "⎪⎢⎜ ∞     ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎪⎢⎜ ⎲     ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎪⎢⎜ ⎳aⁱ-bⁱ⎟⎥⎪");
	fontPrint8(&font, &x, &y, "⎩⎣⎝i=1    ⎠⎦⎭");
	
#endif

	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);

#if 1
	x = 4;
	y = 1;

	fontPrint8(&font, &x, &y, "╔══════════════════════════════════════════╗");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • ‘single’ and “double” quotes         ║");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • Curly apostrophes: “We’ve been here” ║");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • Latin-1 apostrophe and accents: '´`  ║");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • ‚deutsche‘ „Anführungszeichen“       ║");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • †, ‡, ‰, •, 3–4, —, −5/+5, ™, …      ║");
	fontPrint8(&font, &x, &y, "║                                          ║");
	fontPrint8(&font, &x, &y, "║   • ASCII safety test: 1lI|, 0OD, 8B     ║");
	fontPrint8(&font, &x, &y, "║                      ╭─────────╮         ║");
	fontPrint8(&font, &x, &y, "║   • the euro symbol: │ 14.95 € │         ║");
	fontPrint8(&font, &x, &y, "║                      ╰─────────╯         ║");
	fontPrint8(&font, &x, &y, "╚══════════════════════════════════════════╝");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);

#if 1

	x = 4;
	y = 1;
	fontPrint8(&font, &x, &y, "╔══╦══╗  ┌──┬──┐  ╭──┬──╮  ╭──┬──╮  ┏━━┳━━┓");//  ┎┒┏┑   ╷  ╻ ┏┯┓ ┌┰┐    ▊ ╱╲╱╲╳╳╳");
	fontPrint8(&font, &x, &y, "║┌─╨─┐║  │╔═╧═╗│  │╒═╪═╕│  │╓─╁─╖│  ┃┌─╂─┐┃");//  ┗╃╄┙  ╶┼╴╺╋╸┠┼┨ ┝╋┥    ▋ ╲╱╲╱╳╳╳");
	fontPrint8(&font, &x, &y, "║│╲ ╱│║  │║   ║│  ││ │ ││  │║ ┃ ║│  ┃│ ╿ │┃");//  ┍╅╆┓   ╵  ╹ ┗┷┛ └┸┘    ▌ ╱╲╱╲╳╳╳");
	fontPrint8(&font, &x, &y, "╠╡ ╳ ╞╣  ├╢   ╟┤  ├┼─┼─┼┤  ├╫─╂─╫┤  ┣┿╾┼╼┿┫");//  ┕┛┖┚     ┌┄┄┐ ╎ ┏┅┅┓ ┋ ▍ ╲╱╲╱╳╳╳");
	fontPrint8(&font, &x, &y, "║│╱ ╲│║  │║   ║│  ││ │ ││  │║ ┃ ║│  ┃│ ╽ │┃");//  ░░▒▒▓▓██ ┊  ┆ ╎ ╏  ┇ ┋ ▎");
	fontPrint8(&font, &x, &y, "║└─╥─┘║  │╚═╤═╝│  │╘═╪═╛│  │╙─╀─╜│  ┃└─╂─┘┃");//  ░░▒▒▓▓██ ┊  ┆ ╎ ╏  ┇ ┋ ▏");
	fontPrint8(&font, &x, &y, "╚══╩══╝  └──┴──┘  ╰──┴──╯  ╰──┴──╯  ┗━━┻━━┛");//  ▗▄▖▛▀▜   └╌╌┘ ╎ ┗╍╍┛ ┋  ▁▂▃▄▅▆▇█");
	
	y += 10;

	fontPrint8(&font, &x, &y, "  ┎┒┏┑   ╷  ╻ ┏┯┓ ┌┰┐    ▊ ╱╲╱╲╳╳╳");
	fontPrint8(&font, &x, &y, "  ┗╃╄┙  ╶┼╴╺╋╸┠┼┨ ┝╋┥    ▋ ╲╱╲╱╳╳╳");
	fontPrint8(&font, &x, &y, "  ┍╅╆┓   ╵  ╹ ┗┷┛ └┸┘    ▌ ╱╲╱╲╳╳╳");
	fontPrint8(&font, &x, &y, "  ┕┛┖┚     ┌┄┄┐ ╎ ┏┅┅┓ ┋ ▍ ╲╱╲╱╳╳╳");
	fontPrint8(&font, &x, &y, "  ░░▒▒▓▓██ ┊  ┆ ╎ ╏  ┇ ┋ ▎");
	fontPrint8(&font, &x, &y, "  ░░▒▒▓▓██ ┊  ┆ ╎ ╏  ┇ ┋ ▏");
	fontPrint8(&font, &x, &y, "  ▗▄▖▛▀▜   └╌╌┘ ╎ ┗╍╍┛ ┋  ▁▂▃▄▅▆▇█");
	fontPrint8(&font, &x, &y, "  ▝▀▘▙▄▟");
#endif



	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);



#if 1
	x = 4;
	y = 4;
	// runes
	fontPrint8(&font, &x, &y, "ᚻᛖ ᚳᚹᚫᚦ ᚦᚫᛏ ᚻᛖ ᛒᚢᛞᛖ ᚩᚾ ᚦᚫᛗ");
	fontPrint8(&font, &x, &y, "ᛚᚪᚾᛞᛖ ᚾᚩᚱᚦᚹᛖᚪᚱᛞᚢᛗ ᚹᛁᚦ ᚦᚪ ᚹᛖᛥᚫ");
	fontPrint8(&font, &x, &y, " \r\n");
	fontPrint8(&font, &x, &y, "ሰማይ አይታረስ ንጉሥ አይከሰስ።");
	fontPrint8(&font, &x, &y, "ብላ ካለኝ እንደአባቴ በቆመጠኝ።");
	fontPrint8(&font, &x, &y, "ጌጥ ያለቤቱ ቁምጥና ነው።");
	fontPrint8(&font, &x, &y, "ደሀ በሕልሙ ቅቤ ባይጠጣ ንጣት በገደለው።");
	fontPrint8(&font, &x, &y, "የአፍ ወለምታ በቅቤ አይታሽም።");
	fontPrint8(&font, &x, &y, "አይጥ በበላ ዳዋ ተመታ።");
	fontPrint8(&font, &x, &y, "ሲተረጉሙ ይደረግሙ።");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);
	
#if 1
	x = 1;
	y = 1;
	
	fontPrint8(&font, &x, &y, "    &lt;──C0──&gt; &lt;─────────GL──────────&gt;&lt;──C1──&gt; &lt;─────────GR──────────&gt;");
	fontPrint8(&font, &x, &y, "      00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15");
	fontPrint8(&font, &x, &y, "    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐");
	fontPrint8(&font, &x, &y, " 00 │       │SP │                   │       │   │                   │");
	fontPrint8(&font, &x, &y, " 01 │       ├───┘                   │       ├───┘                   │");
	fontPrint8(&font, &x, &y, " 02 │       │                       │       │                       │");
	fontPrint8(&font, &x, &y, " 03 │   C   │       ASCII           │   C   │        Special        │");
	fontPrint8(&font, &x, &y, " 04 │   o   │       graphics        │   o   │        Graphics       │");
	fontPrint8(&font, &x, &y, " 05 │   n   │                       │   n   │                       │");
	fontPrint8(&font, &x, &y, " 06 │   t   │                       │   t   │                       │");
	fontPrint8(&font, &x, &y, " 07 │   r   │                       │   r   │                       │");
	fontPrint8(&font, &x, &y, " 08 │   o   │                       │   o   │                       │");
	fontPrint8(&font, &x, &y, " 09 │   l   │                       │   l   │                       │");
	fontPrint8(&font, &x, &y, " 10 │   s   │                       │   s   │                       │");
	fontPrint8(&font, &x, &y, " 11 │       │                       │       │                       │");
	fontPrint8(&font, &x, &y, " 12 │       │                       │       │                       │");
	fontPrint8(&font, &x, &y, " 13 │       │                       │       │                       │");
	fontPrint8(&font, &x, &y, " 14 │       │                   ┌───┤       │                   ┌───┤");
	fontPrint8(&font, &x, &y, " 15 │       │                   │DEL│       │                   │   │");
	fontPrint8(&font, &x, &y, "    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘");
	fontPrint8(&font, &x, &y, "    &lt;──C0──&gt; &lt;─────────GL──────────&gt;&lt;──C1──&gt; &lt;─────────GR──────────&gt;");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);
		
#if 1
	x = 4;
	y = 4;
	
	fontPrint8(&font, &x, &y, "⎛ n ⎞         n!");
	fontPrint8(&font, &x, &y, "⎜   ⎟ =  ───────────  =  n  ×  (n - 1)");
	fontPrint8(&font, &x, &y, "⎝ 2 ⎠    2! × (n-2)!");
	
	x = 4;
	y += 20;
	fontPrint8(&font, &x, &y, "⡌⠁⠧⠑ ⠼⠁⠒  ⡍⠜⠇⠑⠹⠰⠎ ⡣⠕⠌\r\n");
	
	fontPrint8(&font, &x, &y, "⡍⠜⠇⠑⠹ ⠺⠁⠎ ⠙⠑⠁⠙⠒ ⠞⠕ ⠃⠑⠛⠔ ⠺⠊⠹⠲ ⡹⠻⠑ ⠊⠎ ⠝⠕ ⠙⠳⠃⠞");
	fontPrint8(&font, &x, &y, "⠱⠁⠞⠑⠧⠻ ⠁⠃⠳⠞ ⠹⠁⠞⠲ ⡹⠑ ⠗⠑⠛⠊⠌⠻ ⠕⠋ ⠙⠊⠎ ⠃⠥⠗⠊⠁⠇ ⠺⠁⠎");
	fontPrint8(&font, &x, &y, "⠎⠊⠛⠝⠫ ⠃⠹ ⠹⠑ ⠊⠇⠻⠛⠹⠍⠁⠝⠂ ⠹⠑ ⠊⠇⠻⠅⠂ ⠹⠑ ⠥⠝⠙⠻⠞⠁⠅⠻⠂");
	fontPrint8(&font, &x, &y, "⠁⠝⠙ ⠹⠑ ⠡⠊⠑⠋ ⠍⠳⠗⠝⠻⠲ ⡎⠊⠗⠕⠕⠛⠑ ⠎⠊⠛⠝⠫ ⠊⠞⠲ ⡁⠝⠙");
	fontPrint8(&font, &x, &y, "⡎⠊⠗⠕⠕⠛⠑⠰⠎ ⠝⠁⠍⠑ ⠺⠁⠎ ⠛⠕⠕⠙ ⠥⠏⠕⠝ ⠰⡡⠁⠝⠛⠑⠂ ⠋⠕⠗ ⠁⠝⠹⠹⠔⠛ ⠙⠑");
	fontPrint8(&font, &x, &y, "⠡⠕⠎⠑ ⠞⠕ ⠏⠥⠞ ⠙⠊⠎ ⠙⠁⠝⠙ ⠞⠕⠲\r\n");
	
	fontPrint8(&font, &x, &y, "⡕⠇⠙ ⡍⠜⠇⠑⠹ ⠺⠁⠎ ⠁⠎ ⠙⠑⠁⠙ ⠁⠎ ⠁ ⠙⠕⠕⠗⠤⠝⠁⠊⠇⠲\r\n");

	fontPrint8(&font, &x, &y, "⡍⠔⠙⠖ ⡊ ⠙⠕⠝⠰⠞ ⠍⠑⠁⠝ ⠞⠕ ⠎⠁⠹ ⠹⠁⠞ ⡊ ⠅⠝⠪⠂ ⠕⠋ ⠍⠹");
	fontPrint8(&font, &x, &y, "⠪⠝ ⠅⠝⠪⠇⠫⠛⠑⠂ ⠱⠁⠞ ⠹⠻⠑ ⠊⠎ ⠏⠜⠞⠊⠊⠥⠇⠜⠇⠹ ⠙⠑⠁⠙ ⠁⠃⠳⠞");
	fontPrint8(&font, &x, &y, "⠁ ⠙⠕⠕⠗⠤⠝⠁⠊⠇⠲ ⡊ ⠍⠊⠣⠞ ⠙⠁⠧⠑ ⠃⠑⠲ ⠔⠊⠇⠔⠫⠂ ⠍⠹⠎⠑⠇⠋⠂ ⠞⠕");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);
		
#if 1
	x = 4;
	y = 4;
	
	fontPrint8(&font, &x, &y, "←↑→↓↔↕↖↗↘↙↤↥↦↧↨⇋⇌⇐⇑⇒⇓⇔⇕⇖⇗⇘⇙");
	fontPrint8(&font, &x, &y, "∀∁∂∃∄∅∆∇∈∉∋∌∏∐∑−∓∕∘∙√∝∞∟∠∡∤∥");
	fontPrint8(&font, &x, &y, "∦∧∨∩∪∫∬∮∴∵∼∽≃≅≈≉≒≙≟≠≡≢≤≥≦≧≪≫⊂⊃⊄⊅⊆⊇⊈⊉");
	fontPrint8(&font, &x, &y, "⊊⊋⊕⊗⊤⊥⊦⊧⋀⋁⋂⋃⋅");
	fontPrint8(&font, &x, &y, "⌀⌂⌈⌉⌊⌋⌐⌒⌠⌡〈〉⎷⎸⎹⎺⎻⎼⎽");
	fontPrint8(&font, &x, &y, "␉␊␋␌␍␤");
	fontPrint8(&font, &x, &y, "■□▪▫▬▭▮▲△▶▷►▻▼▽◀◁◄◅◆◇◊○◌◎●◘◙◦◯");
	fontPrint8(&font, &x, &y, "★☆☹☺☻☼♀♁♂♠♡♢♣♤♥♦♧♩♪♫♬♭♮♯");
	fontPrint8(&font, &x, &y, "　、。〃々〆〇〈〉《》「」『』【】〒〓〔〕〜");
	fontPrint8(&font, &x, &y, "⁰ⁱ⁴⁵⁶⁷⁸⁹⁺⁻⁼⁽⁾ⁿ₀₁₂₃₄₅₆₇₈₉₊₋₌₍₎");
	fontPrint8(&font, &x, &y, "ЀЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙКЛМНОПР");
	fontPrint8(&font, &x, &y, "СТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп");
	fontPrint8(&font, &x, &y, "рстуфхцчшщъыьэюяѐёђѓєѕіїјљњћќѝўџҐґ");
	
	// this shows the default replacement charactor (glyph) selected upon an absent charactor request
	// use a CJK font to display these chars
	fontPrint8(&font, &x, &y, "&#12371;&#12435;&#12395;&#12385;&#12399; &#19975;&#22269;&#00088;&#21442;&#21152;&#12398;&#35328;&#00088;&#12363;&#12425;");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(6000);
	fontCleanSurface(&font, NULL);

		
#if 1
	x = 4;
	y = 20;
	uint32_t flags = BFONT_RENDER_ADVANCE_Y | BFONT_RENDER_FIXEDWIDTH | BFONT_RENDER_COMBINED | BFONT_RENDER_RETURN | BFONT_RENDER_NEWLINE;
	flags = fontSetRenderFlags(&font, flags);
	fontPrint8(&font, &x, &y, "W̨̨̛͉̟̜͉ͮ̍͑̚͢ḩ̶̶̢̦̯͉̻̃̔̈́̓̈̍̃͋̉a̡̲̘̲͕͍̩̮̭̦̱̰͍͉ͧ̉̈́ͪ͟͡tͥ̍̒ͩ̾̇ͪ̾ͥ̂̓ͪ̉̉͑͌͛̚҉̷̱̦̯̯̻͚̪̦̩͖̜̼̹̳͉͝͝ͅͅͅ ̼̹̩̼͔̱̘̟̗̲̯̭̤̄̅͗̎͆ͩͣ̋̎ͮ̍̽ͣ͐ͧ͆̽ͮ̐͢͟͝i͓͎̦͍̅̔͗͠͞ͅs͚͈̤̱ͯ̊ͤ̏ͭ͗ͯ̿̊͐̊͑͛͢ ̶̨͖͈̣̺̪̭̺̗͍͔̯̞̪̤͑ͦ̂ͮ̿̉̍̆̊̃̏̀ͧͯ̾͆̂͜w͊ͭ̑́ͫ̈́̄ͩͧ̈́͏̸͖̯̝̯͙̳͇̯͓͉̱̰͡͝h̢̙̗̪̩̥̠ͭ͛͗ͦ͛ͬ̈ͬ̓ͭ͗̋̍̉̕̕a̷͋͗ͩ̾͐͊̊҉̨̧̗̗͙̟̳̻͕̮̱̣͡t̶̘͓̱͓͍̮̫͍̮̟͖̣̱̜͎͚ͪͤ͂̎̎̈́̉ͬͥ͟͝͝ͅ?ͪͫ̅ͫ̆͋͊̇̿̏̂ͦ́́̚҉̶͕̟̞̘̝̺̟͇̩͕̙̱̦͡ͅ");
	fontPrint8(&font, &x, &y, " \r\n");
	fontPrint8(&font, &x, &y, "b̸̛̠͕͉̘͇̻̪̺̰̾̽̓̃͒̊̍͂ͯ̊̐ͬ͟͡͞ͅͅͅe̵̶̸̗͕͈͚̹̻̝͇̲͎̗͑ͦ̈́̋̎̀̐̋̎̎͐͋͂̔͐͋̂ͅḩ̴̨̛̫̻̖̙͚̻̣͉̺̻̭͚͎̻̺̮͙͚ͨͩ͐̾ͫ̿̔̉̏̃̾̊ͬ͐̍ͩ̑̓ͮi̧̼̝̫̭͉͗͑ͫ̈̿̽̍̓͑͛͋ͣͦ̋̑͆̈́ͅn̷̛͓͙͉̪͖̯͈͙̹̏̈̅̓ͮ̀ͅd̴̷͍̼̹͙̥̙̬͚̫̙̥̭̱̜̗̀̔̆͂̓ͥͫ̈͜ ̴̨̪̰̠̮̼͉͕͓̗͑̌ͨ̈́̿ͤ̆͋ͯ͗͆̕ť̵̷̡̛͇̜̜̎͑̌ͬͮ̄̊ͪ͆ͨ̀ͅh̨͔̠͇͖͕͚̣̫̄ͬ̄̾ͤ̑̑͌ͪͣ̍͌̌ͦ̅̆͌̚͟͢͝e̊̈́ͪ͐̌̀͝҉̢̣͎̞̱͙̪̗ ̔̂̊͌̔͗ͫͪ̋̊̇̍҉͏͇̝̭̝͎̞͚̞̳͕͔̯̪͎̫̲̗̯ͅw̷̡̢̠̘̠̙̻̳̞͐ͥͬ̉̿ͤ̇͌͋̿̚a̶͛̉̆́҉̖͇̱͚͙̭̟͓͙̫̘̟͈̫͟l̛̙̳̤̔̓̏͋͆ͮͩ̀ͥ̏ͮ̋͂ͩ̔̌́͘͞l͋ͧͮ̅ͯͭ̔ͯ͆ͩͦ̍͏̷̶̡͇̤̯̖̪̹͓̖́");
	fontPrint8(&font, &x, &y, " \r\n");
	fontPrint8(&font, &x, &y, "S͝͏҉̕U̵̴̕͘C̢̡͜͡͠K̸̨҉͡Y̷̢̕͜ ͟͝͡͡S̴̡̛͞U͝҉͡C̡͟͞͞K͏̛Y̴͝ ̶̛͞L͘͏҉͜O̴̧̡̨̢Ń̷̸̴̕G̀͘͟҉ ̶̢̛͟T̀͢͜Į̧͘͝M̨̧̢͟E̴̡̧͞");
	fontSetRenderFlags(&font, flags);
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
	fontCleanSurface(&font, NULL);



#if 1
	x = 1;
	y = 1;
	fontPrint8(&font, &x, &y, "     COMPUTER A                                COMPUTER B");
	fontPrint8(&font, &x, &y, "┌──────────────────┐                      ┌──────────────────┐");
	fontPrint8(&font, &x, &y, "│ ┌─────────────┐  │                      │  ┌─────────────┐ │");
	fontPrint8(&font, &x, &y, "│ │ Translation │  │      Transfer        │  │ Translation │ │");
	fontPrint8(&font, &x, &y, "│ │ Function:   ├───────────────────────────&gt;│ Function:   │ │");
	fontPrint8(&font, &x, &y, "│ │ FCS to TCS  │  │    Character Set     │  │ TCS to FCS  │ │");
	fontPrint8(&font, &x, &y, "│ └─────────────┘  │                      │  └─────┬───────┘ │");
	fontPrint8(&font, &x, &y, "│       ^          │                      │        │         │");
	fontPrint8(&font, &x, &y, "│       │          │                      │        v         │");
	fontPrint8(&font, &x, &y, "│  Kermit Program  │                      │  Kermit Program  │");
	fontPrint8(&font, &x, &y, "│      SEND        │                      │     RECEIVE      │");
	fontPrint8(&font, &x, &y, "└──────────────────┘                      └────────┬─────────┘");
	fontPrint8(&font, &x, &y, "        ^                                          │");
	fontPrint8(&font, &x, &y, "        │                                          v");
	fontPrint8(&font, &x, &y, "┌───────┴──────────┐                      ┌──────────────────┐");
	fontPrint8(&font, &x, &y, "│  Local File      │                      │  Local File      │");
	fontPrint8(&font, &x, &y, "│  Character Set A │                      │  Character Set B │");
	fontPrint8(&font, &x, &y, "└──────────────────┘                      └──────────────────┘");
#endif


	tft_clearBuffer(COLOUR_WHITE);
	fontApplySurface(&font, 0, 0);
	tft_update();
	delay(2000);
}

