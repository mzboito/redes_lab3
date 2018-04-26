/*Versao UDP modificada pelas alunas Jessica e Marcely */

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#define SOCKET	int
#endif

int main(int argc, char **argv){
	 struct sockaddr_in peer;
	 SOCKET s;
	 int porta, peerlen, rc, i;
	 char ip[16], buffer[1250];
	 float sleep_time, kbps;
	 
#ifdef _WIN32
	 WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("Erro no startup do socket\n");
		exit(1);
	}
#endif

	 if(argc < 5) {
		  printf("Utilizar:\n");
		  printf("trans -h <numero_ip> -p <porta>\n");
		  exit(1);
	 }

	 // Pega parametros
	 for(i=1; i<argc; i++) {
		  if(argv[i][0]=='-') {
				switch(argv[i][1]) {
					 case 'h': // Numero IP
						  i++;
						  strcpy(ip, argv[i]);
						  break;

					 
					 case 'p': // porta
						  i++;
						  porta = atoi(argv[i]);
						  if(porta < 1024) {
								printf("Valor da porta invalido\n");
								exit(1);
						  }
						  break;
					 case 'b': //Kbps do meio 
					 	  i++;
						  kbps = atoi(argv[i]) * 1024; //transforms in Kbps
						  break;
					 default:
						  printf("Parametro invalido %d: %s\n",i,argv[i]);
						  exit(1);
				}		  	 
		  } else {
			  printf("Parametro %d: %s invalido\n",i, argv[i]); 
				exit(1);
		  }
	 }
	 
// Cria o socket na familia AF_INET (Internet) e do tipo UDP (SOCK_DGRAM)
	 if((s = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		  printf("Falha na criacao do socket\n");
		  exit(1);
 	 }
	 
// Cria a estrutura com quem vai conversar 
	 peer.sin_family = AF_INET;
	 peer.sin_port = htons(porta);
	 peer.sin_addr.s_addr = inet_addr(ip); 
	 peerlen = sizeof(peer);
	
// Calcula o sleep time
	sleep_time = (float) 1000000 / kbps;
	
strcpy(buffer,"UM PACOTE");

// Envia pacotes <3
	while(1)
	{

		sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&peer, peerlen);
		usleep(sleep_time);

		/*#ifdef _WIN32
				rc = recvfrom(s,buffer,sizeof(buffer),0,(struct sockaddr *)&peer, &peerlen); 
				printf("Recebido %s\n\n",&buffer);
				Sleep(5000);
		#else //recebe o ack
				//rc = recvfrom(s,buffer,sizeof(buffer),0,(struct sockaddr *) &peer,(socklen_t *) &peerlen); 
				//printf("Recebido %s\n\n",&buffer);
				
		#endif*/
	}
}
