/*-------------------------------------------------------------------
**
**  Fichero:
**    leds.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las implementaciones de las funciones
**    para el acceso a los leds de la placa de prototipado
**
**  Notas de dise�o:
**    Equivale al fichero hom�nimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "leds.h"

// Mascaras de bits
#define LED1 0x01
#define LED2 0x02
#define BIT_LED1 (0x1 << 9)
#define BIT_LED2 (0x1 << 10)

// status representa el estado de los leds
// s�lo tienen sentido los dos bits menos
// significativos, cada uno para un led
// 1 encendido y 0 apagado
static unsigned int status = 0;

// Configura puerto B para usar los leds como salida
// Enciende los leds de acuerdo al valor de status
void leds_init( void ){
	rPCONB = 0x1ff;
	rPDATB = 0x000;
	leds_display(status);
	//status = 0x3;
}

// Modificar status para que el LED1 cambie su estado
// Actualizar el estado de los leds usando leds_display
void switchLed1( void )
{
	status ^= LED1;
	leds_display( status );
}

// Modificar status para que el LED2 cambie su estado
// Actualizar el estado de los leds usando leds_display
void switchLed2( void )
{
	status ^= LED2;
	leds_display( status );
}


void led1_on( void )
{
	status &=  ~LED1;
	leds_display( status );
}

void led1_off( void )
{
	status |=  LED1;
	leds_display( status );
}

void led2_on( void )
{
	status &=  ~LED2;
	leds_display( status );
}

void led2_off( void )
{
	status |=  LED2;
	leds_display( status );
}


void leds_switch( void ){
	status ^= (LED1 | LED2);
	leds_display( status );
}

void leds_display( unsigned int leds_status )
{
	status = leds_status;
	
	// LED 1
	if( status & LED1 )
		rPDATB &= ~BIT_LED1;
	else
		rPDATB |= BIT_LED1;

	// LED 2
	if( status & LED2 )
		rPDATB &= ~BIT_LED2;
	else
		rPDATB |= BIT_LED2;
}
