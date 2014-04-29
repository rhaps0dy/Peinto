#ifndef _readme_h_
#define _readme_h_

#include <cstdio>

void showREADME()
{
	const char readme[] = "Controles\n"
		"---------\n"
		"A               - Añadir emisor bajo el cursor\n"
		"D               - Borrar emisor seleccionado\n"
		"Click           - Seleccionar un emisor\n"
		"Arrastrar ratón - Mover el emisor seleccionado\n"
		"Q / Z           - Longitud de onda + / -\n"
		"W / X           - Amplitud + / - (puede haber overflow de color)\n"
		"E / C           - Velocidad de propagación + / -\n"
		"P               - Pausar / continuar tiempo del emisor seleccionado\n";
	puts(readme);
}
#endif 
