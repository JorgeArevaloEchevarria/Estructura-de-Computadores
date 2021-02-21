/*-------------------------------------------------------------------
**
**  Fichero:
**    timer.h  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de funciones
**    para la gesti�n del timer 0 del chip S3C44BOX
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#ifndef _TIMER_H_
#define _TIMER_H_

/* configura el timer y lo arranca, debe ser invocada al comienzo */
void timer_init( void ); 
/* activa el timer, sin modificar el valor mostrado en el display */
void timer_start(void);
/* detiene el timer, no se producen m�s interrupciones del tiemer */
void timer_stop(void);
/* si el timer est� activo lo para, si est� parado lo activa */
void timer_switch(void);

/* reinicia la cuenta mostrada en el display */
void timer_clear(void);

#endif
