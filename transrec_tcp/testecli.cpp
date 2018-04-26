// testecli.cpp //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
/*#ifdef _WIN32
	#include <winsock2.h>
#else*/
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define	SOCKET	int
	#define INVALID_SOCKET  ((SOCKET)~0)
//#endif
#include <ctime>


int main(int argc, char* argv[])
{
  SOCKET s;
  char STR_IPSERVIDOR[256];
  int PORTA_SRV, PORTA_CLI, i; 
  struct sockaddr_in  s_cli, s_serv;
  
  if(argc < 7) {
		  printf("Utilizar:\n");
		  printf("trans -h <numero_ip> -s <porta_srv> -c <porta_cli>\n");
		  exit(1);
	 }

	 // Pega parametros
	 for(i=1; i<argc; i++) {
		  if(argv[i][0]=='-') {
				switch(argv[i][1]) {
					 case 'h': // Numero IP
						  i++;
						  strcpy(STR_IPSERVIDOR, argv[i]);
						  break;

					 
					 case 's': // porta servidor
						  i++;
						  PORTA_SRV = atoi(argv[i]);
						  if(PORTA_SRV < 1024) {
								printf("Valor da porta invalido\n");
								exit(1);
						  }
						  break;
					 case 'c': // porta cliente
					 	  i++;
						  PORTA_CLI = atoi(argv[i]);
              if(PORTA_CLI < 1024) {
								printf("Valor da porta invalido\n");
								exit(1);
						  }
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

  /*#ifdef _WIN32
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
      printf("Erro no startup do socket\n");
      exit(1);
    }
  #endif*/
  // abre socket TCP
  if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
  {
    printf("Erro iniciando socket\n");
    return(0);
  }

  // seta informacoes IP/Porta locais
  s_cli.sin_family = AF_INET;
  s_cli.sin_addr.s_addr = htonl(INADDR_ANY);
  s_cli.sin_port = htons(PORTA_CLI);

  // associa configuracoes locais com socket
  if ((bind(s, (struct sockaddr *)&s_cli, sizeof(s_cli))) != 0)
  {
    printf("erro no bind\n");
    close(s);
    return(0);
  }

  // seta informacoes IP/Porta do servidor remoto
  s_serv.sin_family = AF_INET;
  s_serv.sin_addr.s_addr = inet_addr(STR_IPSERVIDOR);
  s_serv.sin_port = htons(PORTA_SRV);

  // connecta socket aberto no cliente com o servidor
  if(connect(s, (struct sockaddr*)&s_serv, sizeof(s_serv)) != 0)
  {
    printf("erro na conexao");
    close(s);
    exit(1);
  }

  int buffer_size = 1250; // tamanho pacote
  char str[buffer_size]; // criacao do buffer
  int package_count; // count para os pacotes
  int iter = 0; // count de iteracao
  time_t t_now, t_init; // variaveis de controle de tempo
  time(&t_init); //init tempo
  while(1) {
    if ((send(s, (const char *)&str, sizeof(str),0)) < 0) {
      printf("erro na transmiss�o\n");
      close(s);
      return 0;
    }
    package_count++;
    time(&t_now);
    if(difftime(t_now, t_init) >= 1.0){
      iter++;
      printf("%d %d\n", iter, package_count*buffer_size);
      package_count = 0;
    }
  }
  // fecha socket e termina programa
  printf("Fim da conexao\n");
  close(s);
  return 0;
}
