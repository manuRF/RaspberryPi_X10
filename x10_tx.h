/*
	Manuel Rodriguez 27/07/2016
  Instalacion general: KODI (OSMC)
  Se han añadido las librerias wiringPi y lirc.
  persianas funciona como un proceso arrancado en systemctl y
  que espera conexiones en un socket de red puerto 104 y señales
  IR que provienen de un mando EUSKALTEL para controlar por RF con
  (protocolo X10) a las dos persianas del SALON 
*/
#ifndef _x10tx_h
#define _x10tx_h
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>
#include <string.h>
/*
******************      sockets   ***************************************
*/
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
# include <signal.h>
/*
* ***********************************************************************
*/ 
void mydelay(int delay_us);
void lock(void);
void cero(void);
void uno(void);
void silencio(void);
void addr_comm(int addr, int comm);
int encoder(char *string_data);
void  char2binary(char *string);
void  string2binary(char *string);
//void send_comand(char *comando);
/*
* ***********************************************************************
*/
	struct X10Code{
		char code[8];
		char comm[16];
		char end;
	};	
/*
* ***********************************************************************
*/
#endif