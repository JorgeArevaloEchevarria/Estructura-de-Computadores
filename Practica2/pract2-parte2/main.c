/*-------------------------------------------------------------------
**
**  Fichero:
**    main.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Implementa un contador start/stop
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#include "leds.h"
#include "D8Led.h"
#include "button.h"
#include "timer.h"


/**** IMPLEMENTAR DE LA PARTE 1 *******/
void intcont_init(void) {
	rINTCON = 0x0;
	rINTMOD = 0x0;
	rI_ISPC = 0xfffff;
	rEXTINTPND =  0xf;
	rINTMSK = 0x3ffffff;
}

int main(void)
{	
	/* Configura el controlador de interrupciones */
	intcont_init();

	leds_init();
	led2_off();
	led1_on();

	D8Led_init();
	D8Led_digit( 0 );  	/* muestra el simbolo inicial en el display */
	Delay(0);

	button_init();
	timer_init();

    while(1);

    return 0;
}
