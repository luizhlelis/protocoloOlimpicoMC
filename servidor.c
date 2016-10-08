#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define MAXBUF 100

int main(int argc, char **argv){

        char *MsgRead = malloc (MAXBUF*sizeof(char));
        char *MsgWrite = malloc (MAXBUF*sizeof(char));
        int  *ClientSocket = malloc (30*sizeof(int));
        struct sockaddr_in6 ServerAddr, ClientAddr;
        int Addrlen = sizeof(ClientAddr);
        int Sd =-1, Port, NewSocket, MaxClients = 30, Activity, ValRead, MaxSd, SdClient;

        // Conjunto de descritores de um socket
        fd_set ReadFds;

        Port = atoi(argv[1]);
        int Position, i, TimeMiliseconds = 1;
        Record AthleteTime;

        Sd = socket(AF_INET6, SOCK_STREAM, 0);

        /* Foi atribuido in6addr_any ao endereço pois ele permite (por padrao) estabelecer conexoes
        de qualquer cliente IPv4 e IPv6 que especificar a porta igual ao segundo argumento(argv[2])*/

        memset(&ServerAddr, 0, sizeof(ServerAddr));
        ServerAddr.sin6_family = AF_INET6;
        ServerAddr.sin6_port   = htons(Port);
        ServerAddr.sin6_addr   = in6addr_any;

        bind(Sd,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

        listen(Sd, 5); // Permite que o servidor aceite conexoes de entrada do cliente

        Table *AthletesPos = malloc(sizeof(Table));

        while(1){

            // Limpa o conjunto
            FD_ZERO(&ReadFds);

            // Adiciona o socket principal
            FD_SET(Sd, &ReadFds);
            MaxSd = Sd;

            for (i=0; i < MaxClients; i++){

                // Descritor de socket
                SdClient = ClientSocket[i];

                // Adiciona os sockets filhos ao conjunto
                if(SdClient > 0)
                    FD_SET(SdClient, &ReadFds);

                // O maior numero do descritor de arquivo e' armazenado para ser usado futuramente na funcao select()
                if(SdClient > MaxSd)
                    MaxSd = SdClient;
            }

            // Espera infinitamente por uma atividade em um dos sockets
            Activity = select( MaxSd+1, &ReadFds, NULL, NULL, NULL);

            // Se algo acontece no socket principal, e' uma conexao chegando
            if (FD_ISSET(Sd, &ReadFds)){
                NewSocket = accept(Sd,(struct sockaddr*)&ClientAddr, &Addrlen);  //Aceita uma solicitacao de conexao de entrada

                // Adiciona o novo socket ao array
                for (i=0; i < MaxClients; i++){
                    // Se posicao vazia
                    if ( ClientSocket[i] == 0 ){
                        ClientSocket[i] = NewSocket;
                        break;
                    }
                }
            }


            //Caso contrario, e' alguma operacao de IO em outro socket
            for(i = 0; i < MaxClients; i++){
                SdClient = ClientSocket[i];

                if(FD_ISSET(SdClient, &ReadFds)){
                    //Confere se foi fechamento e tambem le as msgs chegando

                    ValRead = read(SdClient, MsgRead, MAXBUF);
                    TimeMiliseconds = ChangeToMilisec(MsgRead, strlen(MsgRead));

                    if((ValRead == 0)){
                        //Nao foi lido nenhum byte em read - informa e fecha
                        close(SdClient);
                        ClientSocket[i] = 0;
                    }
                    else if(TimeMiliseconds<0){
                        //Fecha a conexao de um cliente
                        sprintf(MsgWrite,"%d", TimeMiliseconds);
                        write(SdClient, MsgWrite, strlen(MsgWrite)+1);
                        close(SdClient);
                        ClientSocket[i] = 0;
                        strcpy(MsgRead, "");
                        strcpy(MsgWrite, "");
                    }
                    else{
                        //Responde a msg que chegou para o respec cliente
                        AthleteTime.Key = TimeMiliseconds;
                        InsertTable(AthleteTime, AthletesPos);
                        InsertionSort(AthletesPos->Item, AthletesPos->n);
                        Position = BinarySearch(AthleteTime.Key, AthletesPos);
                        sprintf(MsgWrite,"%d", Position);
                        write(SdClient, MsgWrite, strlen(MsgWrite)+1);
                    }

                }
            }

        }

        free(AthletesPos);
        free(MsgRead);
        free(MsgWrite);

}
