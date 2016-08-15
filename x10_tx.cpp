/*
	Manuel Rodriguez 27/07/2016
  Instalacion general: KODI (OSMC)
  Se han añadido las librerias wiringPi y lirc.
  persianas funciona como un proceso arrancado en systemctl y
  que espera conexiones en un socket de red puerto 104 y señales
  IR que provienen de un mando EUSKALTEL para controlar por RF con
  (protocolo X10) a las dos persianas del SALON 
*/
# include "ir2x10.h"
# include "x10_tx.h"

using namespace std;
#define OUT2RF 3
/*
* ***********************************************************************
* Delay general de usec, se necesita precision para los tiempos 
* del protcolo RF X10
*/
void mydelay(int delay_us)
{
	long int start_time;
	long int time_difference;
	struct timespec gettime_now;

	clock_gettime(CLOCK_REALTIME, &gettime_now);
	start_time = gettime_now.tv_nsec;
	while (1)
	{
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0)
			time_difference += 1000000000;
		if (time_difference > (delay_us * 1000))
			break;
	}
}
/*
* **********************************************************************
* Señal inicial del protocolo X10 ()
*/
void send_lock(void)
{
	digitalWrite (OUT2RF, HIGH) ;	// On
	mydelay(9000);
    	digitalWrite (OUT2RF, LOW) ;	// Off
 	mydelay(4500);
}
/*
* ***********************************************************************
*/
void send_cero(void)
{
	digitalWrite (OUT2RF, HIGH) ;   // On
    	mydelay(400) ;
    	digitalWrite (OUT2RF, LOW) ;	// Off
   	mydelay(800) ;
}
/*
* ***********************************************************************
*/
void send_one(void)
{
	digitalWrite (OUT2RF, HIGH) ;   // On
    	mydelay(400) ;
   	digitalWrite (OUT2RF, LOW) ;	// Off
   	mydelay(2000) ;
}
/*
* ***********************************************************************
*/
void silencio(void)
{
	digitalWrite (OUT2RF, HIGH) ;   // On
	mydelay(400) ;
	digitalWrite (OUT2RF, LOW) ;    // Off
	mydelay(32000) ;
}
/*
* ***********************************************************************
*/
void send_locksend_onesend_ceroCoding(int addr, int comm){
	int bit, count,mask,loop;
	for(loop=0;loop<6;loop++){
		send_lock();
		mask = 0x80;
		for(count =1 ; count<=8; count++){// Saca el primer octeto tal cual
			bit = addr & mask;
			if (bit > 0){
				send_one();
			}else {
				send_cero();
			}
			mask = mask/2;
		}
		mask = 0x80;
		for(count =1 ; count<=8; count++){// Saca el primer octeto comoplementado
			bit = addr & mask;
			if (bit > 0){
				 send_cero();
			} else {
				send_one();
			}
			mask = mask/2;
		}
		mask = 0x80;
			for(count =1 ; count<=8; count++){ // Saca el segundo octeto tal cual
			bit = comm & mask;
			if (bit > 0) {
				send_one();
			} else {
					send_cero();
			}
			mask = mask/2;
		}
		mask = 0x80;
		for(count =1 ; count<=8; count++){// Saca el segundo octeto complementado
			bit = comm & mask;
			if (bit > 0) {
				send_cero();
			} else {
					send_one();
			}
			mask = mask/2;
		}
		silencio();
	}
}
/*
* ***********************************************************************
*/
string long2String(long mylong){
    string mystring;
    stringstream mystream;
    mystream << mylong;
    return mystream.str();
}
/*
* ***********************************************************************
*/
int string2number(char *string_data){
	int Numb_data;
	if(strcmp(string_data,"0000")== 0) Numb_data = 0;
	if(strcmp(string_data,"0001")== 0) Numb_data = 1;
	if(strcmp(string_data,"0010")== 0) Numb_data = 2;
	if(strcmp(string_data,"0011")== 0) Numb_data = 3;
	if(strcmp(string_data,"0100")== 0) Numb_data = 4;
	if(strcmp(string_data,"0101")== 0) Numb_data = 5;
	if(strcmp(string_data,"0110")== 0) Numb_data = 6;
	if(strcmp(string_data,"0111")== 0) Numb_data = 7;
	if(strcmp(string_data,"1000")== 0) Numb_data = 8;
	if(strcmp(string_data,"1001")== 0) Numb_data = 9;
	if(strcmp(string_data,"1010")== 0) Numb_data = 10;
	if(strcmp(string_data,"1011")== 0) Numb_data = 11;
	if(strcmp(string_data,"1100")== 0) Numb_data = 12;
	if(strcmp(string_data,"1101")== 0) Numb_data = 13;
	if(strcmp(string_data,"1110")== 0) Numb_data = 14;
	if(strcmp(string_data,"1111")== 0) Numb_data = 15;
	return Numb_data;
}
/*
* ***********************************************************************
*/
void  string2binary(char *string){
	cout<<"comando x10:"<<string<<endl;
	char Fist[5], Second[5], Third[5], Four[5];
	int  FistNumb, SecondNumb, ThirdNumb, FourNumb;
	int count;
	Fist[4] = '\0'; Second[4] = '\0' ; Third[4] ='\0'; Four[4] ='\0';
	for (count = 0; count <= 3 ; count++)	Fist[count]   = string[count];
	for (count = 4; count <= 7 ; count++)	Second[count-4] = string[count];
	for (count = 8; count <= 11; count++)	Third[count-8] = string[count];
	for (count = 12;count <= 15; count++)	Four[count-12]= string[count];

	FistNumb   = string2number(Fist);
	SecondNumb = string2number(Second);
	ThirdNumb  = string2number(Third);
	FourNumb   = string2number(Four);
	int addr   = FistNumb*16 + SecondNumb;
	int comm   = ThirdNumb*16 + FourNumb;
	send_locksend_onesend_ceroCoding(addr,comm);
}
/*
* ***********************************************************************
*/

