#include "platform.h"
#include "framework.h"
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
		"ESC         - Cancela la acción actual, termina una línea\n"
		"              poligonal, termina una forma\n"
		);
}
