


#include "common.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

static _ufont_t font;




//#define fontFile	UFDIR"unicode36.uf"
#define fontFile	UFDIR"10x20.uf"
//#define fontFile	UFDIR"unifont32.uf"
//#define fontFile	UFDIR"freesans32.uf"




static PROGMEM const char *entities[] = {
	"&lt; ", "&gt; ", "&le; ", "&ge; ", "&ne; ", "&or; ", "&ni; ", "&Mu; ", 
	"&Nu; ", "&Xi; ", "&Pi; ", "&mu; ", "&nu; ", "&xi; ", "&pi; ", "&amp; ", 
	"&yen; ", "&rho; ", "&tau; ", "&phi; ", "&chi; ", "&psi; ", "&eta; ", 
	"&piv; ", "&Rho; ", "&uml; ", "&not; ", "&shy; ", "&reg; ", "&deg; ", 
	"&Eta; ", "&Tau; ", "&Phi; ", "&Chi; ", "&Psi; ", "&and; ",
	/*"&zwj; ", 
	"&lrm; ", 		space glyphs
	"&rlm; ", */
	"&ang; ", "&cap; ", "&cup; ", "&int; ", "&sim; ", "&sub; ", "&sup; ", 
	"&sum; ", "&loz; ", "&ETH; ", "&eth; ", "&quot; ", "&euro; ", "&nbsp; ", 
	"&bull; ", "&apos; ", "&circ; ", 
	/*"&ensp; ", 
	"&emsp; ", 		 space glyphs
	"&zwnj; ",*/
	"&cent; ", "&real; ", "&larr; ", "&uarr; ", "&rarr; ", "&darr; ", 
	"&harr; ", "&lArr; ", "&uArr; ", "&rArr; ", "&dArr; ", "&hArr; ", 
	"&isin; ", "&part; ", "&prod; ", "&prop; ", "&cong; ", "&nsub; ", 
	"&sube; ", "&supe; ", "&perp; ", "&sdot; ", "&lang; ", "&rang; ", 
	"&sect; ", "&copy; ", "&ordf; ", "&macr; ", "&para; ", "&ordm; ",
	"&sup1; ", "&sup2; ", "&sup3; ", "&fnof; ", "&Beta; ", "&Zeta; ", 
	"&Iota; ", "&beta; ", "&zeta; ", "&iota; ", "&Yuml; ", "&Auml; ", 
	"&Ouml; ", "&Euml; ", "&Iuml; ", "&Uuml; ", "&auml; ", "&euml; ", 
	"&iuml; ", "&ouml; ", "&uuml; ", "&yuml; ", "&ucirc; ", "&thorn; ", 
	"&sigma; ", "&theta; ", "&kappa; ", "&omega; ", "&upsih; ", "&OElig; ", 
	"&oelig; ", "&Acirc; ", "&Aring; ", "&AElig; ", "&Alpha; ", "&Gamma; ", 
	"&Delta; ", "&Theta; ", "&Kappa; ", "&Sigma; ", "&Omega; ", "&alpha; ", 
	"&gamma; ", "&delta; ", "&iexcl; ", "&diams; ", "&clubs; ", "&lceil; ", 
	"&rceil; ", "&oplus; ", "&minus; ", "&radic; ", "&infin; ", "&asymp; ", 
	"&equiv; ", "&exist; ", "&empty; ", "&nabla; ", "&notin; ", "&pound; ", 
	"&tilde; ", "&ndash; ", "&mdash; ", "&lsquo; ", "&rsquo; ", "&sbquo; ", 
	"&ldquo; ", "&rdquo; ", "&bdquo; ", "&prime; ", "&Prime; ", "&oline; ", 
	"&frasl; ", "&image; ", "&trade; ", "&crarr; ", "&laquo; ", "&times; ", 
	"&acute; ", "&micro; ", "&cedil; ", "&raquo; ", "&Ucirc; ", "&Ecirc; ", 
	"&Icirc; ", "&Ocirc; ", "&THORN; ", "&szlig; ", "&acirc; ", "&aring; ", 
	"&aelig; ", "&ecirc; ", "&icirc; ", "&ocirc; ", "&ccedil; ", "&egrave; ", 
	"&eacute; ", "&igrave; ", "&iacute; ", "&ntilde; ", "&ograve; ", "&oacute; ", 
	"&otilde; ", "&oslash; ", "&ugrave; ", "&uacute; ", "&Igrave; ", "&Iacute; ", 
	"&Ntilde; ", "&Ograve; ", "&Oacute; ", "&Otilde; ", "&Oslash; ", "&Ugrave; ", 
	"&Uacute; ", "&Yacute; ", "&agrave; ", "&aacute; ", "&atilde; ", "&plusmn; ", 
	"&divide; ", "&middot; ", "&frac14; ", "&frac12; ", "&frac34; ", "&iquest; ", 
	"&spades; ", "&hearts; ", "&brvbar; ", "&curren; ", "&thinsp; ", "&dagger; ", 
	"&Dagger; ", "&permil; ", "&lsaquo; ", "&rsaquo; ", "&hellip; ", "&weierp; ", 
	"&forall; ", "&there4; ", "&lowast; ", "&otimes; ", "&vellip; ", "&lfloor; ", 
	"&rfloor; ", "&Lambda; ", "&sigmaf; ", "&lambda; ", "&Scaron; ", "&scaron; ", 
	"&Agrave; ", "&Aacute; ", "&Atilde; ", "&Ccedil; ", "&Egrave; ", "&Eacute; ", 
	"&yacute; ", "&Epsilon; ", "&Omicron; ", "&Upsilon; ", "&epsilon; ", 
	"&omicron; ", "&upsilon; ", "&alefsym; ", "&thetasym; ", ""
};

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	sd_init();
	tft_init();

	if (!fontOpen(&font, fontFile)){
		//printf("fontOpen() failed \r\n");
		return;
	}
		
	tft_clear(COLOUR_WHITE);
	tft_update();
	
	// create a 1BPP work surface. this is the initial work surface
	_ufont_surface_t *surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_24TO16(0x777777), NULL);
	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	fontSetRenderSurface(&font, surface);

	fontSetLineSpace(&font, fontGetLineSpace(&font)-1);
	fontSetRenderFlags(&font, BFONT_RENDER_NEWLINE|BFONT_RENDER_NODEFAULT|BFONT_RENDER_ADVANCE_Y/*|BFONT_RENDER_GLYPHRECT*/);

	char buffer[256] = {0};
	int total = sizeof(entities) / sizeof(*entities);
	
	
	int x = 0;
	int y = 1;
	
	for (int i = 0; i < total; ){
		buffer[0] = 0;
		for (int c = 0; c < 23 && i < total; c++)
			strcat(buffer, entities[i++]);

		fontPrint(&font, &x, &y, (uint8_t*)buffer);
		//fontDrawLine(&font, 0, y, DWIDTH-1, y, 1);
	}


	fontApplySurface(&font, 0, 0);
	tft_update();
	 
	fontSurfaceFree(surface);
	fontClose(&font);
}

void loop ()
{

	delay(200);
}

