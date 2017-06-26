#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFFTAM 1024  //Tamanho maximo do buffer
#define PORTA 8888   //A porta na qual sera enviado os dados

void terminar(char *s)
{
    perror(s);
    exit(1);
}

void IniciarCliente()
{
    struct sockaddr_in servidorsock;/*definindo as variaveis*/
    int s, slen=sizeof(servidorsock);
    char buf[BUFFTAM];
    char mensagem[BUFFTAM];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        terminar("socket");
    }

    memset((char *) &servidorsock, 0, sizeof(servidorsock));
    servidorsock.sin_family = AF_INET;
    servidorsock.sin_port = htons(PORTA);

    if (inet_aton(SERVER , &servidorsock.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(strcmp(buf,"fim\n")!=0){
        printf("Digite uma mensagem : ");
        fgets(mensagem,BUFFTAM,stdin);

        //Envia a mensagem
        if (sendto(s, mensagem, strlen(mensagem) , 0 , (struct sockaddr *) &servidorsock, slen)==-1){
            terminar("sendto()");
        }

        //recebe uma mensagem de volta e printa ela na tela
        //limpa o buffer de dados
        memset(buf,'\0', BUFFTAM);
        //tenta receber dados do servidor
        if (recvfrom(s, buf, BUFFTAM, 0, (struct sockaddr *) &servidorsock, (socklen_t *)&slen) == -1){
            terminar("recvfrom()");
        }
        //printa os dados recebidos
        printf("%s\n",buf);
    }

    close(s);
}

int main()
{
    IniciarCliente();
    return 0;
}
