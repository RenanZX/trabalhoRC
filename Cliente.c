#include <stdio.h>
#ifdef __WIN32__
#include <winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") /*Winsock Biblioteca*/
 
#define SERVER "127.0.0.1"  /*endereço IP do servidor UDP*/
#define BUFLEN 512  /*tamanho maximo do buffer*/
#define PORTA 8888   /*a porta que receberá os dados enviados do servidor*/

void StartCliente(){
	struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    WSADATA wsa;
 
    //Inicializa o winsock
    printf("\nInicializando Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Falha. Codigo de Erro : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Inicializado.\n");
     
    //cria o socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() falha com codigo de erro : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
     
    //estrutura do endereço
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORTA);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
     
    //começa a comunicacao com servidor
    while(1)
    {
        printf("Digite uma mensagem : ");
        gets(message);
         
        //envia a mensagem
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() falhou codigo de erro : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
         
        //recebe uma resposta e imprime na tela
        //limpa o buffer
        memset(buf,'\0', BUFLEN);
        //tenta receber algum dado
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() falhou codigo de erro : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
         
        puts(buf);
    }
 
    closesocket(s);
    WSACleanup();

}

#else
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

	void StartCliente(){
		int clientSocket, nBytes;
  		char buffer[1024];
  		struct sockaddr_in serverAddr;
  		socklen_t addr_size;

  		/*Cria um socket UDP*/
  		clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  		/*Configura a estrutura de endereço do servidor*/
  		serverAddr.sin_family = AF_INET;
  		serverAddr.sin_port = htons(7891);
  		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  		/*Inicializa o tamanho da variavel que será ultilizada depois*/
  		addr_size = sizeof serverAddr;

  		while(1){
    		printf("Digite a mensagem que sera enviada ao servidor:\n");
    		fgets(buffer,1024,stdin);
    		printf("Voce digitou: %s",buffer);

    		nBytes = strlen(buffer) + 1;
    
    		/*Envia a mensagem para o servidor*/
    		sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);

    		/*Recebe a mensagem do servidor*/
        	nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);

    		printf("Mensagem recebida do servidor: %s\n",buffer);

  		}
	}

#endif

int main(){
	StartCliente();	
	return 0;
}