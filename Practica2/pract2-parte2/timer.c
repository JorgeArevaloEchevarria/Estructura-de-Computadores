/*-------------------------------------------------------------------
**
**  Fichero:
**    timer.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las implementaciones de las funciones
**    para la gestión del timer 0 del chip S3C44BOX
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "leds.h"
#include "D8Led.h"
#include "timer.h"

#define MCLK 64000000

static int symbol = 0;     /* variable de estado del símbolo del contador software */
static int timer_stat = 0; /* variable de estado del timer (1 activo, 0 inactivo)   */

static void timer_ISR(void) __attribute__ ((interrupt ("IRQ")));

/*
 * Debe incremetar el valor mostrado por el display de 8
 * segmentos (symbol) y mostrar el nuevo valor por el display.
 * Cuando la cuenta llegue a 9 se quedará en 9.
 */
static void timer_ISR(void){

	if (symbol < 9) {
		symbol++;
		leds_switch();
		D8Led_digit(symbol);
	} else {
		timer_stop();
	}
	rI_ISPC = BIT_TIMER2;
}

/*
 * Debe arrancar el timer2 para que genere una interrupción a los dos segundos
 * y en modo autoreload: valor de cuenta 62500, con pre-escalado y divisor
 * configurados en timer_init (no hace falta volver a configurarlo aquí). 
 * Para arrancar el timer:
 * 	- activar bit de manual update
 * 	- descativar bit de manual update
 * 	- activar bits de auto-reload y start
 *
 * 	Los dos últimos pasos se pueden fusionar en una única instrucción C.
 *
 * 	Antes de arrancar deberíamos borrar posibles interrupciones pendientes y
 * 	desenmascarar.
 *
 * 	Los comentarios dan una idea de lo que hay que ir haciendo.
 */

void timer_start(void){
    /* manual update */
	rTCON = 0x2000;
	/* Se borra el bit de interrupción pendiente del TIMER2 */
	rI_ISPC = BIT_TIMER2;
	/* not manual update, start and auto-reload*/
	rTCON = 0x9000;
	/* Se borra el bit de interrupción pendiente del TIMER2 */
	rI_ISPC = BIT_TIMER2;
	
	timer_stat = 1;
	/* Desenmascara la linea del timer 2 y el bit global */
	rINTMSK &= ~(BIT_GLOBAL | BIT_TIMER2);
}

/*
 * Debe parar el timer 2 y enmascarar su línea de interrupción */
void timer_stop(void)
{
	// Implementar
	rTCON = ~(1 << 12); // bit 0
	rINTMSK |= BIT_TIMER2; // enmascara timer2

	timer_stat = 0;
}

void timer_switch(void) {
	if (timer_stat) {
		timer_stop();
	} else {
		timer_start();
	}
}

void timer_clear(void) {
	timer_stop();
	symbol = 0;
	D8Led_digit(symbol); 		     
}

/* 
 * Debe configurar el timer 2 para que produzca interrupciones cada 
 * 2 segundos: preescalado 255 y divisor 8.
 * Al final se invocará a timer_start para que se inicie. Los comentarios
 * dan una idea de lo que hay que ir haciendo.
 */
void timer_init(void){
	// Implementar
	
	// Establecer la rutina de servicio para TIMER2

   /* f = MCLK / ((P+1) * D)
    * para una interrupción cada 2 segundos: N * 1/f = 2 con N <= 65535  
	* N * ((P+1)*D) / MCLK = 2 => P = 2 * MCLK / (N*D) - 1
	* Tomamos N como el mayor divisor de 2*CLK/D => para D = 8 y MCLK = 64MHz, N = 65200
    * y P = 2*64000000/(62500*8) - 1 = 255, que cabe justo en los 8 bits de pre-escalado 
    */
	/* divisor 1/8 */

	/* pre-escalado 255 */
	
	/* Dejar el timer parado */
	pISR_TIMER2 = (unsigned) timer_ISR;
	rTCFG0 = 0X0000FF00;
	rTCFG1 = 0X0000200;
	rTCNTB2 = 65200;
	rTCON = ~(1 << 12);



	timer_stat = 0;

	timer_start();
}

