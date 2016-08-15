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

#define OUT2RF 3
#define A2ON    "0110000000010000"
#define A2OFF   "0110000000110000"
#define A3ON    "0110000000001000"
#define A3OFF   "0110000000101000"
#define ABRIGHT "0110000010001000"
#define ADIM    "0110000010011000"
#define PORT	104

bool movingUP;
bool movingDOWN;

 using namespace std;
/*
* ***********************************************************************
*/ 
int main(int argc, char *argv[])
{
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = kb_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	int sockfd, newsockfd, portno;
	pid_t pid_net, pid_lirc;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	struct lirc_config *config;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
	// Inicializacion para el GPIO de salida de RF
    if (wiringPiSetupGpio () == -1)
		{
			cout<<"Fallo al activar libreria wiringPi"<<endl;
			exit (EXIT_FAILURE) ;
		}
       
	pinMode(OUT2RF, OUTPUT); 
	
	// Para matar los zombies del forck
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) 
		{
			cout<< "error" << SIG_ERR <<endl;
			exit(EXIT_FAILURE);
		}
		// Initiate net socket for Android clients.
	if (sockfd < 0)
		{
			cout<<"ERROR abriendo el socket"<<endl;
			exit(EXIT_FAILURE);
		}   
		
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        cout<<"ERROR enlazando al puerto :"<< portno<<endl;
	else cout<< "Socket creado y enlazado en el puerto :"<< portno<<endl;	
    listen(sockfd,5);
    clilen = sizeof(cli_addr);	
			cout<< "net socket inicializado OK"<<endl;
	//Initiate LIRC. Exit on failure
    if(lirc_init((char*)"lirc",1)==-1)
		{
		cout<<"Fallo inicializando lirc"<<endl;
		exit(EXIT_FAILURE);
		}
     //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
	bool lircdresult = lirc_readconfig(NULL,&config,NULL);
	if(lircdresult==0)		
		cout<<"Lectura de lircd.conf cargada y OK:"<<lircdresult<<endl;		
 	else cout<<"fallo en la lectura de lircd.conf"<<lircdresult<<endl;
	
	pid_lirc = fork();
	if(pid_lirc < 0)
		cout<<"ERROR en la creacion de tarea IR"<<endl;
	// Lanza el proceso LIRC de entrada del DIODO IR
	if(pid_lirc == 0)
		{
			proc_lirc();
			exit(0);
		}
	while (1) 
	{
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		cout<<"aceptada la conexion"<<endl;
		if (newsockfd < 0)
			cout<<"ERROR en la aceptacion"<<endl;
		pid_net = fork();
		if (pid_net < 0)
			cout<<"ERROR en la creacion de tarea NET"<<endl;
		if (pid_net == 0)  
			{
			close(sockfd);
			proc_net(newsockfd);
			exit(0);
		}else close(newsockfd);
				
	} // end of while
		//Frees the data structures associated with config.
		lirc_freeconfig(config);
		exit(EXIT_FAILURE);
}
/*
* ***********************************************************************
*/
void proc_lirc(void)
{
	int buttonTimer = millis();
	char *code;
    char *c;

	while(lirc_nextcode(&code)==0)
	{	
		//If code = NULL, meaning nothing was returned from LIRC socket,
        //then skip lines below and start while loop again.		
        if(code==NULL) continue;
		{

            //Make sure there is a 400ms gap before detecting button presses.
        if (millis() - buttonTimer  > 400)
			{
				cout<<"codigo encontrado:"<<code<<endl;
                if(strstr (code,"KEY_2")) //Intenta encontrar KEY_2 en el string code
					{
						cout<<"Econtrado  KEY_2: A3ON "<<endl;
						movingUP = true;
						movingDOWN = false;
						string2binary((char *)A3ON);
						buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_1"))
					{
						cout<<"Econtrado  KEY_1: A3OFF"<<endl;
						movingUP = false;
						movingDOWN = true;
						string2binary((char *)A3OFF);
						buttonTimer = millis();
					}
                    else if(strstr (code,"KEY_4"))
					{
						cout<<"Econtrado  KEY_4: A2ON"<<endl;
						movingUP = true;
						movingDOWN = false;
						string2binary((char *)A2ON);
						buttonTimer = millis();
                    }
					else if(strstr (code,"KEY_3"))
					{
						cout<<"Econtrado  KEY_3 :A2OFF"<<endl;
						movingUP = false;
						movingDOWN = true;
						string2binary((char *)A2OFF);
						buttonTimer = millis();
					}
					else if (strstr (code,"KEY_5"))
					{
						if (movingUP==true) 
						{
							cout<<"KEY_5 :ABRIGHT MovingUp"<<endl;
							string2binary((char *)ABRIGHT);
						}
						else if(movingDOWN==true) 
						{
							cout<<"KEY_5 :ADIM MovingDown"<<endl;
							string2binary((char *)ADIM);
						}
					}
					else if (strstr (code,"KEY_6"))
					{
						cout<<"Econtrado  KEY_6 :ADIM"<<endl;
						string2binary((char *)ADIM);
					}
					else if (strstr (code,"KEY_7"))
					{
						cout<<"Econtrado  KEY_7 :ABRIGHT"<<endl;
						string2binary((char *)ABRIGHT);
					}
			}
			cout<<"saliendo de en proclirc() desde code = NULL"<<endl;
           //Need to free up code before the next loop
		}
            free(code);		
        } 
		cout<<"Saliendo solo si se a pulsado IR"<<endl;
}
/*
* ***********************************************************************
*/
void kb_handler(int s){
           printf("Interrupcion de teclado %d\n",s);
           exit(1); 
}
/*
* ***********************************************************************
*/
void proc_net (int sock)
{
   int n;
   char comando[16] ;
   bzero(comando,16);
   n = read(sock,comando,sizeof(comando));
   if (n < 0) 
	   cout<<"ERROR reading from socket"<<endl;
   else 
	   cout<<"tarea comando :"<<comando<<endl;
   string2binary((char *)comando);
}  
/*
* ***********************************************************************
*/