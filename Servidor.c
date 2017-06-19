#include <stdio.h>
#ifdef __WIN32__
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define BUFFLEN 512
#define PORTA 8888

void IniciarServer(){
	SOCKET s;
    struct sockaddr_in server, si_other;
    int slen , recv_len;
    char buf[BUFFLEN];
    WSADATA wsa;
 
    slen = sizeof(si_other) ;
     
    //Inicializa o winsock
    printf("\nInicializando Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Falha. Codigo de Erro : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Inicializado.\n");
     
    //Cria um socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Não foi possivel criar o socket : %d" , WSAGetLastError());
    }
    printf("Socket Criado.\n");
     
    //Preparada a estrutura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORTA );
     
    //Faz o Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Falha no bind com codigo de erro : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind feito");
 
    //Espera para o recebimento de dados
    while(buf!="fim")
    {
        printf("Esperando por Dados...");
        fflush(stdout);
         
        //limpa os dados recebidos anteriormente no buffer
        memset(buf,'\0', BUFFLEN);
         
        //tenta receber dados do cliente
        if ((recv_len = recvfrom(s, buf, BUFFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() falhou com o codigo de erro : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
         
        //imprime os detalhes do cliente e os dados recebidos
        printf("Pacote recebido de %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Dados: %s\n" , buf);
        memset(buf,'\0',BUFFLEN);
        printf("Digite o feedback ao cliente:");
        gets(buf);
        getchar();
         
        //retorna ao cliente com os mesmos dados que foram enviados pelo mesmo
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() falhou com codigo de erro : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
    }
 	/*fecha e limpa o socket criado*/
    closesocket(s);
    WSACleanup();
}

#else
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#define SIZEBUF 1024

	void IniciarServer(){
		int udpSocket, nBytes;
  		char buffer[SIZEBUF];
  		struct sockaddr_in serverAddr;
  		struct sockaddr_storage serverStorage;
  		socklen_t addr_size;
  		int i;

  		/*Cria um socket UDP*/
  		udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  		/*Configura os endereços da struct*/
  		serverAddr.sin_family = AF_INET;
  		serverAddr.sin_port = htons(7891);
  		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  		/*Faz o bind do socket com endereço da struct*/
  		bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  		/*Inicializa o tamanho da variavel que sera ultilizada depois*/
  		addr_size = sizeof serverStorage;

  		/*enquanto estiver rodando*/
  		while((strcmp(buffer,"FIM")!=0)&&(strcmp(buffer,"fim")!=0)){
  			printf("servidor Iniciado...");
    		/* Tenta receber um datagrama UDP. O endereço e a porta do cliente 
    		que iniciou o pedido ira ser passada para a variavel serverStorage*/
    		nBytes = recvfrom(udpSocket,buffer,SIZEBUF,0,(struct sockaddr *)&serverStorage, &addr_size);

    		printf("mensagem recebida:%s",buffer);
    		printf("pressione enter para continuar...\n");
    		getchar();
        	memset(buffer,'\0', SIZEBUF);
        	if((strcmp(buffer,"FIM")!=0)&&(strcmp(buffer,"fim")!=0)){
        		printf("digite uma mensagem:");
    			fgets(buffer,sizeof(buffer),stdin);
    			getchar();
    			/*Converte a mensagem recebida em letras maiúsculas*/
    			for(i=0;i<nBytes-1;i++){
      				buffer[i] = toupper(buffer[i]);
      			}
    			/*Envia a mensagem de volta para o cliente, ultilizando serverStorage como endereço*/
    			sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
        	
        	}else{
        		printf("cliente encerrou a conexao");
        	}
  		}
	}
#endif

int main(){
	IniciarServer();
	return 0;
}