#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#define BUFFTAM 1024
#define PORTA 8888

void terminar(char *s)
{
    perror(s);
    exit(1);
}

void IniciarServer(){
    struct sockaddr_in servidorsock, cliente;
    int s, slen = sizeof(cliente);

    char buf[BUFFTAM];

    //cria um scoket udp
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        terminar("socket");
    }

    // zera a estrutura
    memset((char *) &servidorsock, 0, sizeof(servidorsock));

    servidorsock.sin_family = AF_INET;
    servidorsock.sin_port = htons(PORTA);
    servidorsock.sin_addr.s_addr = htonl(INADDR_ANY);

    //faz o bind do socket com a porta
    if( bind(s , (struct sockaddr*)&servidorsock, sizeof(servidorsock) ) == -1){
        terminar("bind");
    }

    //espera por dados
    while(strcmp(buf,"fim\n")!=0){
        printf("Esperando por dados\n");
        fflush(stdout);
        memset(buf,'\0',BUFFTAM);

        //tenta receber algum dado do cliente
        if (recvfrom(s, buf, BUFFTAM, 0, (struct sockaddr *) &cliente, (socklen_t*)&slen) == -1){
            terminar("recvfrom()");
        }

        //imprime os dados recebidos
        printf("Origem do cliente %s:%d\n", inet_ntoa(cliente.sin_addr), ntohs(cliente.sin_port));
        printf("mensagem: %s\n" , buf);

        printf("Digite a mensagem que deseja enviar ao cliente : ");
        fgets(buf,BUFFTAM,stdin);

        //retorna o cliente com a mensagem
        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &cliente, slen) == -1){
            terminar("sendto()");
        }
    }

}


int main()
{
    IniciarServer();
    return 0;
}
