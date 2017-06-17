#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
void tcp_s(){
	struct sockaddr_in addrport;
	struct sockaddr clientAddr;
	int s;
	int msgLen = 1024;
	char msg[msgLen]; 
	socklen_t addrlen = sizeof(clientAddr);
	int	sockid;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	if(sockid < 0){
		cout <<"Error stablishing connection"<<endl;
		exit(1);
	}
	cout<<"Server Socket connection created...."<<endl;
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(1500);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockid, (struct sockaddr*)&addrport, sizeof(addrport)) < 0){
		cout << "Error binding socket..." << endl;
		exit(1);
	}
	cout << "Looking for clients...." << endl;
	listen(sockid, 1);
	s = accept(sockid, &clientAddr, &addrlen);
	if(s < 0){
		cout << "Error on accepting..." << endl;
		exit(1);
	}
	while(s > 0){
		strcpy(msg, "Server connected...\n");
		send(s, msg, msgLen, 0);
		cout << "connected whith client..." <<endl;
		cout << "Enter 'fim' to end the connection" <<endl;
		do{
			cout << "client: ";
			recv(s, msg, msgLen, 0);
			if(strcmp(msg, "fim") == 0){
				cout << "Connection terminated..."<<endl;
				close(sockid);
				exit(1);	
			}
			cout << msg<<endl;
			cout << "Server: ";
			fgets(msg, msgLen, stdin);
			msg[strlen(msg) - 1] = '\0';
			send(s, msg ,msgLen, 0);
			if(strcmp(msg, "fim") == 0){
				cout << "Connection terminated..."<<endl;
				close(sockid);
				exit(1);	
			}
		}while(1);
	}
}
int main(){
	tcp_s(); 
	return 0;
}