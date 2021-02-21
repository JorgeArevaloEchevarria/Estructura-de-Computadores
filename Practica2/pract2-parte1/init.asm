/*-----------------------------------------------------------------
**
**  Fichero:
**    init.asm  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Arranca un programa en C
**
**  Notas de dise�o:
**
**---------------------------------------------------------------*/

    .extern main
    .extern ISR_Undef
	.extern ISR_SWI
	.extern ISR_Pabort
	.extern ISR_Dabort
	.extern ISR_IRQ
	.extern DoDetecta

	.global start
	
	.global DoUndef
    .global DoSWI
    .global DoDabort

/* Tabla en la que escribir las direcciones de las ISRs */
.equ 	_ISR_STARTADDRESS,	0xc7fff00		/* GCS6:64M DRAM/SDRAM 	*/
.equ    HandleReset,    _ISR_STARTADDRESS
.equ    HandleUndef,    _ISR_STARTADDRESS+4
.equ    HandleSWI,      _ISR_STARTADDRESS+4*2
.equ    HandlePabort,   _ISR_STARTADDRESS+4*3
.equ    HandleDabort,   _ISR_STARTADDRESS+4*4
.equ    HandleReserved, _ISR_STARTADDRESS+4*5
.equ    HandleIRQ,      _ISR_STARTADDRESS+4*6
.equ    HandleFIQ,      _ISR_STARTADDRESS+4*7

/* Constantes utiles para la rutina ISR_FIQ */
.equ INTPND, 0x1e00004
.equ EXTINTPND, 0x1d20054
.equ F_ISPC, 0x1e0003c


	/*
	** Modos de operaci�n
	*/
	.equ MODEMASK, 0x1f		/* Para selecci�n de M[4:0] del CPSR */
	.equ USRMODE,  0x10
	.equ FIQMODE,  0x11
	.equ IRQMODE,  0x12
	.equ SVCMODE,  0x13
	.equ ABTMODE,  0x17
	.equ UNDMODE,  0x1b
	.equ SYSMODE,  0x1f

	/*
	** Direcciones de las bases de las pilas del sistema 
	*/
	.equ USRSTACK, 0xc7ff000   	
	.equ SVCSTACK, 0xc7ff100
	.equ UNDSTACK, 0xc7ff200
	.equ ABTSTACK, 0xc7ff300
	.equ IRQSTACK, 0xc7ff400
	.equ FIQSTACK, 0xc7ff500

	/*
	** Registro de m�scara de interrupci�n
	*/
	.equ rINTMSK,    0x1e0000c

start:

	/* Pasa a modo supervisor */
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #SVCMODE
    msr	cpsr_c, r1 

	/* Enmascara interrupciones */
	ldr r0, =rINTMSK
	ldr r1, =0x1fffffff
    str r1, [r0]

	/* Habilita linea IRQ y FIQ del CPSR */
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr_c, r0
	
	/* Desde modo SVC inicializa los SP de todos los modos de ejecuci�n privilegiados */
    bl InitStacks
	
	/* Esscribe en la tabla de ISR las direcciones de las rutinas de tratamiento de excepcion*/
	bl InitISR

	/* Desde modo SVC cambia a modo USR e inicializa el SP_usr */
	mrs r0, cpsr
	bic r0, r0, #MODEMASK
	orr r1, r0, #USRMODE  
	msr cpsr_c, r1
	ldr sp, =USRSTACK

    bl main

End:
    B End

/** COMPLETAR **/
InitStacks:

   // Pasar a modo undef e inicializar su stack
    mrs	r0, cpsr//cargas el valor del registro de estado en r0
    bic	r0, r0, #MODEMASK//desenmascaras
    orr	r1, r0, #UNDMODE//escribes los bits del modo que quieras inicializar la pila
    msr	cpsr_c, r1//escribes en el registro de estado el valor del modo deseado
    ldr sp, =UNDSTACK//inicializas la pila del estado deseado

	/* Modo abort */
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #ABTMODE
    msr	cpsr_c, r1
    ldr sp, =ABTSTACK

    /* Modo irq*/
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #IRQMODE
    msr	cpsr_c, r1
    ldr sp, =IRQSTACK

	/*Modo FIQ*/
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #FIQMODE
    msr	cpsr_c, r1
    ldr sp, =FIQSTACK

    /*Modo svc */

    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #SVCMODE
    msr	cpsr_c, r1
    ldr sp, =SVCSTACK

	mov pc, lr

/** COMPLETAR **/
InitISR:

	/*ISR Undef */
	ldr r0, =ISR_Undef//carga de la direccion de memoria donde debe empieza el tratamiento de excepcion
	ldr r1, =HandleUndef//cargamos la direccion del handle pertinente
	str r0, [r1]//cargamos en el handle la direccion de la rutina que trata la excepcion

	/*ISR Dabort (Data abort)*/
	ldr r0, =ISR_Dabort
	ldr r1, =HandleDabort
	str r0, [r1]

	/*ISR Pabort (Prefetch abort)*/
	ldr r0, =ISR_Pabort
	ldr r1, =HandlePabort
	str r0, [r1]

	/*ISR IRQ */
	ldr r0, =ISR_IRQ
	ldr r1, =HandleIRQ
	str r0, [r1]

	/*ISR SWI */
	ldr r0, =ISR_SWI
	ldr r1, =HandleSWI
	str r0, [r1]

	/*ISR FIQ */
	ldr r0, =ISR_FIQ
	ldr r1, =HandleFIQ
	str r0, [r1]

	mov pc,lr


/** COMPLETAR
** 1. comprobar si hay interrupcion pendiente por EINT4567 consultando INTPND (por ser FIQ)
** 2. Comprobar si ha sido uno de los dos botones, consultando EXTINPD
** 3. Si es asi, saltar a DoDetecta
** 4. Antes de finalizar, limpiar interrupciones pendientes escribiendo en F_ISPC
** 5. Hacer el retorno de acuerdo a la tabla presentada en el guion
**/
ISR_FIQ:push {r0-r10, fp, lr}
		ldr r0, =INTPND//comprobamos si hay alguna interrupcion
		ldr r0, [r0]
		mov r2, #0x0200000
		and r2, r0, r2
		cmp r2, #0x0200000//comparamos con la mascara adecuada para comprobar el bit 21
		bne terminado//si no esta a 1 el bit 21 no hay interrupcion y salimos
		ldr r0, =EXTINTPND//si hay interrupcion comprobamos que bits estan a 1
		ldr r0, [r0]
		mov r2, #4
		cmp r0,r2//si es igual a 100  o a 1000 hay algun boton pulsado y por lo tanto hemos de saltar a pulsado
		beq pulsado
		mov r2,#8
		cmp r0,r2
		beq pulsado

		b terminado

pulsado: bl DoDetecta

terminado:ldr r0, =F_ISPC
		 ldr r0, [r0]//se carga el valor del registro de interrupciones pendientes
		 mov r3, #0xf
		 orr r3, r0, r3//or con mascara pertinente
		 ldr r0, =F_ISPC
		 str r3, [r0]//limpiamos el registro de interrupciones pendientes
		 ldr r1,=EXTINTPND
		// mov r3,#0xf
		 str r3,[r0]
		 pop {r0-r10, fp, lr}
		 subs pc, lr, #4


DoSWI:
	swi #0
	mov pc,lr

DoUndef:
	.word 0xE6000010
	mov pc,lr

DoDabort:
	ldr r0,=0x0a333333
	str r0,[r0]
	mov pc,lr


	.end

