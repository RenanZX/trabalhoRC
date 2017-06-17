#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
void tcp_c(){
	int sockid;
	int portNum = 1500;
	int msgLen = 1024;
	char msg[msgLen];
	struct sockaddr_in foreignAddr;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	if(sockid < 0){
		cout<< "Error creating socket...." << endl;
		exit(1);
	}
	cout << "Client socket created...."<<endl;
	foreignAddr.sin_family = AF_INET;
	foreignAddr.sin_port = htons(portNum);
	if(connect(sockid, (struct sockaddr*)&foreignAddr, sizeof(foreignAddr)) == 0){
		cout<< "Connecting to server...."<<endl;
	}
	recv(sockid, msg, msgLen, 0);
	cout << "Connection confirmed"<<endl;
	cout <<"Enter 'fim' to end the Connection"<<endl;
	do{
		cout << "Client: ";
		fgets(msg, msgLen, stdin);
		msg[strlen(msg) - 1] = '\0';
		send(sockid, msg, msgLen, 0);
		if(strcmp(msg, "fim") == 0){
			cout << "Connection terminated..."<<endl;
			close(sockid);
			exit(1);	
		}
		cout << "Server: ";
		recv(sockid, msg, msgLen, 0);
		if(strcmp(msg, "fim") == 0){
			cout << "Connection terminated..."<<endl;
			close(sockid);
			exit(1);	
		}
		cout << msg<<endl;
	}while(1);
}
int main(){
	tcp_c();
	return 0;
}