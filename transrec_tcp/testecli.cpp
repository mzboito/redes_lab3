// testecli.cpp //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define	SOCKET	int
	#define INVALID_SOCKET  ((SOCKET)~0)
#endif
#include <ctime>


#define PORTA_CLI 2345 // porta TCP do cliente
//#define PORTA_SRV 2023 // porta TCP do servidor
#define STR_IPSERVIDOR "127.0.0.1"
//#define STR_IPSERVIDOR "192.168.0.146"

int main(int argc, char* argv[])
{
  SOCKET s;
  int PORTA_SRV = atoi(argv[1]);
  struct sockaddr_in  s_cli, s_serv;
  
#ifdef _WIN32
	 WSADATA wsaData;
  
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("Erro no startup do socket\n");
		exit(1);
	}
#endif
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
    //printf("erro na conexao - %d\n", WSAGetLastError());
    printf("erro na conexao");
    close(s);
    exit(1);
  }

#if 0
  // envia mensagem de conexao - aprimorar para dar IP e porta
  if ((send(s, "Conectado\n", 11,0)) == SOCKET_ERROR);
  {*
    printf("erro na transmiss�o - %d\n", WSAGetLastError());
    closesocket(s);
    return 0;
  }
#endif

  // recebe do teclado e envia ao servidor
  int str_size = 1250;
  char str[str_size];
  //char ch;
  //int i;
  int m_count;
  int start_s; // the code you wish to time goes here
  int stop_s;
  float time;
  FILE *pFile;

  start_s=clock();
  while(1)
  {

    if ((send(s, (const char *)&str, sizeof(str),0)) < 0)
    {
      //printf("erro na transmiss�o - %d\n", WSAGetLastError());
      printf("erro na transmiss�o\n");
      close(s);
      return 0;
    }
    m_count++;
    stop_s=clock();
    time = (stop_s-start_s)/double(CLOCKS_PER_SEC);
    if(time >= 1){
        if((pFile = fopen(argv[1],"a"))==NULL){
            perror("Error opening file.");
            exit(1);
        }else{
            fprintf(pFile, "%d\n", m_count*str_size);
            m_count = 0;
            start_s=clock();
        }
    }
     //usleep(100000);
  }

  // fecha socket e termina programa
  printf("Fim da conexao\n");
  close(s);
  return 0;
}
