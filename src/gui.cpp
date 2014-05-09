#include "platform.h"
#include "framework.h"
#include "image.h"
#include "gui.h"

#include <stdio.h>

void
showREADME(void)
{
	puts(
		"Instrucciones\n"
		"-------------\n"
		"L(ine)      - Dibuja una línea\n"
		"C(ircle)    - Dibuja un círculo\n"
		"P(olygonal) - Dibuja varias líneas seguidas\n"
		"S(hape)     - Dibuja una forma (polígono)\n"
		"F(reehand)  - Dibuja a mano alzada\n"
		"B(ucket)    - Bucket fill the clicked region\n"
		"ESC         - Cancela la acción actual\n"
		"RETURN      - Termina la línea poligonal o cierra la forma\n"
		"Q(uit)      - Sal del programa\n"
		);
}

void
drawColorSelector(ColorSelector *cs, Image *img)
{
	Uint rech, recw, x, basex, y, basey, i;
	Color col;

	if(!cs->visible) return;
	rech = cs->h - 4*cs->margins;
	if(rech==0 || rech > img->height) return;
	rech /= 3;
	recw = cs->w - 2*cs->margins;
	if(recw==0 || rech > img->height) return;

	//blue bar on bottom
	for(i=0; i<3; i++) {
		col = cs->c;
		basex = cs->x + cs->margins;
		basey = cs->y + cs->margins*(i+1) + rech*i;
		col.v[2-i] = 0;
		for(x=0; x < recw; x++) {
			col.v[2-i] = x*255/recw;
			for(y=basey; y < rech+basey; y++)
				img->setPixel(basex+x, y, col);
		}
		for(x=0; x<3; x++) col.v[x] = 255 - cs->c.v[x];
		for(y=basey; y < rech+basey; y++)
			img->setPixel(basex+ cs->c.v[2-i]*recw/255, y, col);
	}
}

char
handleClick(ColorSelector *cs, Pos2 p)
{
	Uint i, basex, basey, rech;
	basex = cs->x + cs->margins;
	basey = cs->y + cs->margins;
	rech = (cs->h - 4*cs->margins)/3;
	for(i=0; i<3; i++) {
		if(((p.x >= basex) &&
			(p.x < cs->x - cs->margins + cs->w)) && (
			(p.y >= basey + cs->margins*i + rech*i) &&
			(p.y < basey + cs->margins*i + rech*(i+1))))
		{
			cs->c.v[2-i] = (p.x-basex)*255/(cs->w - 2*cs->margins);
			return -1;
		}
	}
	return 0;
}
