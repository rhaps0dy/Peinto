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
		"L(ine)      - Dibuja una l�nea\n"
		"C(ircle)    - Dibuja un c�rculo\n"
		"P(olygonal) - Dibuja varias l�neas seguidas\n"
		"S(hape)     - Dibuja una forma (pol�gono)\n"
		"F(reehand)  - Dibuja a mano alzada\n"
		"ESC         - Cancela la acci�n actual, termina una l�nea\n"
		"              poligonal, termina una forma\n"
		);
}
