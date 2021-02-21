/*-------------------------------------------------------------------
**
**  Fichero:
**    button.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las implementaciones de las funciones
**    para la gestión de los pulsadores de la placa de prototipado
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "button.h"
#include "leds.h"
#include "timer.h"
#include "utils.h"

static void button_ISR(void) __attribute__ ((interrupt ("IRQ")));

/* Configurar el puerto G para que:
 * - Se registre button_ISR para tratar interrupciones de la linea EINT4567
 * - Se soliciten interrupciones por flanco de bajada en los pines 6 y 7
 * - Se active el pull-up en los pines 6 y 7
 * - Queden borradas posibles interrupciones pendientes de la línea EINT4567
 * - Quede desenmascarada la línea EINT4567
 *
 * Los comentarios dan una indicación de lo que se debe ir haciendo.
 */
void button_init( void ){
	// Implementar
	

	/* ESTABLECE LA RUTINA DE SERVICIO PARA EINT4567*/
	pISR_EINT4567 = (unsigned) button_ISR;

	/* pines 6 y 7 generan interrupciones EINT6 y EINT7 */
	//rINTMOD |= BIT_EINT4567;
	rPCONG = 0xf000;

	/* EINT 6 y 7 generan interrupciones por flanco de bajada*/
	rEXTINT = 0x22000000;
	rEXTINTPND = 0xf;
	/* activado el pull-up para los pines 6 y 7 */
	rPUPG = 0x0;
	/* borramos posibles interrupciones pendientes */
	rI_ISPC = 0xfffff;
	/* Desenmascara las lineas eint4567 y el bit global */
	rINTMSK &= ~(BIT_EINT4567 | BIT_GLOBAL);

}

#define BUT1 0x4
#define BUT2 0x8

/* 
 * Es la rutina de tratamiento de interrupción de los botones. Tras esperar
 * para eliminar posibles rebotes, debe identificar si se ha pulsado el botón 1
 * o el botón 2, y realizar la operación correspondiente, según lo indicado por
 * el enunciado.
 */
static void button_ISR(void){
    /* Idenficiar la interrupcion */
    unsigned int which_int = rEXTINTPND;

	/* Espera fin de rebote de presión, 200 ms */
    Delay( 2000 );
	
	// Si se ha pulsado el botón 1 arrancar/parar el timer
	// si estaba parado/en marcha. Ver API del timer.

	// Si se ha pulsado el botón 2 parar y resetear el timer
	// Ver API del timer.

    switch (which_int) {
    		case 4:
    			timer_switch();
    			break;
    		case 8:
    			timer_clear();
    			break;
    		default:
    			break;
       }

    /* Finalizar ISR, borrando el bit de petición de interrupción, tanto
	 * en el puerto G (extintpnd) como en el controlador de interrupciones
	 */
	rEXTINTPND = 0xf;
	rI_ISPC= ~(0x0);

}





