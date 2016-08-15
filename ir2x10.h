/*
	Manuel Rodriguez 27/07/2016
  Instalacion general: KODI (OSMC)
  Se han añadido las librerias wiringPi y lirc.
  persianas funciona como un proceso arrancado en systemctl y
  que espera conexiones en un socket de red puerto 104 y señales
  IR que provienen de un mando EUSKALTEL para controlar por RF con
  (protocolo X10) a las dos persianas del SALON 
*/
#ifndef _ir2x10_h
#define _ir2x10_h
/*
* ***********************************************************************
*/
#include <wiringPi.h>
#include <lirc/lirc_client.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>
/*
* ***********************************************************************
*/
using namespace std;
/*
* ***********************************************************************
*/
void setupPins(); 
void proc_net (int sock);
void kb_handler(int s);
void proc_lirc(void);
/*
* ***********************************************************************
*/
#endif