#include "myserver.h"

int main(){
	server_init();

	int client_id=0;
	while(1){
		int client_socket;
		struct sockaddr_in socket_addr;
		unsigned int len = sizeof(sockaddr_in);
		if((client_socket = accept(listening_socket,(struct sockaddr *)&socket_addr,&len))==-1){
			perror("accept error: ");
			exit(-1);
		}
		thread t(handle_client, client_id, client_socket, socket_addr);
		t.detach();
		client_id++;
	}
	close(listening_socket);
	return 0;
}