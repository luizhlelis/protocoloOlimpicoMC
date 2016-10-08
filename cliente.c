#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "funcoes.h"

#define MAXBUF 100

int main(int argc, char **argv){

	int    Sd=-1, Rc;
    struct in6_addr ServerAddr;
    struct addrinfo Hints, *Res=NULL;
    char *MsgWrite = malloc (MAXBUF*sizeof(char));
    char *MsgRead = malloc (MAXBUF*sizeof(char));

    memset(&Hints, 0x00, sizeof(Hints));
    Hints.ai_flags = AI_NUMERICSERV;
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;

    Rc = inet_pton(AF_INET, argv[1], &ServerAddr);
    if (Rc == 1){ // endereco valido de IPv4?
       Hints.ai_family = AF_INET;
       Hints.ai_flags = AI_NUMERICHOST;
    }
    else{
       Rc = inet_pton(AF_INET6, argv[1], &ServerAddr);
       if (Rc == 1){ // endereco valido de IPv6?
          Hints.ai_family = AF_INET6;
          Hints.ai_flags = AI_NUMERICHOST;
       }
    }

    getaddrinfo(argv[1], argv[2], &Hints, &Res); //Pega a informacao do endereco para o servidor

    // A funcao socket() retorna um descritor de sockets que representa um endpoint.
    Sd = socket(Res->ai_family, Res->ai_socktype, Res->ai_protocol);

    // Usa a função connect () para estabelecer uma conexão com o servidor.
    connect(Sd, Res->ai_addr, Res->ai_addrlen);

    do{
        ReadAndEditStr(MsgWrite, MAXBUF);
        write(Sd, MsgWrite, strlen(MsgWrite)+1);
        read(Sd, MsgRead, MAXBUF);
        if(atoi(MsgRead)>=0)
            printf("%s\n", MsgRead);
    } while(atoi(MsgRead)>=0);

    close(Sd);
    freeaddrinfo(Res);
    free(MsgWrite);
    free(MsgRead);

}
