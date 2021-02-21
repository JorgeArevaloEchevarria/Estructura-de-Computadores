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
**    Genera y trata una colección de excepciones/interrupciones
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "leds.h"
#include "button.h"
#include "utils.h"

char Screen[1024];

extern void DoSWI  (void);
extern void DoUndef(void);
extern void DoDabort(void);

void write(char* text, char* address);

void ISR_SWI(void) __attribute__ ((interrupt ("SWI")));
void ISR_Undef(void) __attribute__ ((interrupt ("UNDEF")));
void ISR_Pabort(void) __attribute__ ((interrupt ("ABORT")));
void ISR_Dabort(void) __attribute__ ((interrupt ("ABORT")));
void ISR_IRQ(void) __attribute__ ((interrupt ("IRQ")));


void write(char* text, char* address)
{
	while( *text != 0 )
	  *address++ = *text++;
}

void ISR_IRQ(void)
{
	write("IRQ   ",Screen);
}

void ISR_Undef(void)
{
    write("Undef ",Screen);
}


void ISR_SWI(void)
{
    write("SWI   ",Screen);
}

void ISR_Pabort(void)
{
    write("Pabort",Screen);
}

void ISR_Dabort(void)
{
    write("Dabort",Screen);
}


// Configuracion por defecto del controlador de interrupciones:
// - Modo vectorizado, IRQ y FIQ habilitadas
// - Configurar todas las interrupciones por IRQ
// - Borrar pendientes por IRQ y FIQ
// - Borrar EXTINPND
// - Enmascarar todas las líneas, pero activar BIT_GLOBAL
void intcont_init(void){
	rINTCON = 0x0;
	rINTMOD = 0x0;
	rI_ISPC = 0xfffff;
	rEXTINTPND =  0xf;
	rINTMSK = 0x3ffffff;
}


int main(void){

	intcont_init();
	leds_init();
	button_init();
	Delay(0);
    DoUndef();
    DoSWI();
    DoDabort();

	while(1);
	return (0);
}
