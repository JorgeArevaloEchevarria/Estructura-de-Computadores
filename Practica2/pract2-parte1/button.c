/*-------------------------------------------------------------------
**
**  Fichero:
**    button.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las implementaciones de las funciones
**    para la gesti�n de los pulsadores de la placa de prototipado
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#include "button.h"
#include "leds.h"


// Configuraci�n de la botonera
// - Habilitar EINT4567 por FIQ
// - Configura puerto G para generar interrupciones por EINT4567 por flanco de bajada (PCONG - EXTINT)
// - Configurar resistencia de pull-up
// - Desenmascarar el bit EINT4567 para permitr interrupciones por esa linea
void button_init( void )
{
	rINTMOD |= BIT_EINT4567;
	rPCONG = 0xf000;
	rEXTINT = 0x22000000;//01x01x!
	rPUPG = 0x0;
	rINTMSK &= ~(BIT_EINT4567);


}

// COMPLETAR LA FUNCION
// Almacenar en which_int el origen de la interrupcion:
//  - 4 para un boton, 8 para otro
// Al finalizar, borrar la interrupci�n en EXTINTPND
void DoDetecta(void) 
{
	int which_int;

	/* Identificar la interrupcion */
	which_int = rEXTINTPND;

	/* Espera fin de rebote de presi�n */
	// Delay hace espera activa. Tiempo expresado en unidades de 100us (2000 -> 200 ms)

	Delay( 2000 );


	/* Encender/apagar el LED adecuado */
	switch (which_int) {
		case 4:
			switchLed1();
			break;
		case 8:
			switchLed2();
			break;
		default:
			break;
   }

   /* Finalizar ISR */
	rEXTINTPND = 0xf;
	rF_ISPC= ~(0x0);

}

