#include "headers.h"
int client_socket;
struct sockaddr_in server_socket_addr; 
thread t_recv_messages;

void catch_ctrl_c(int signal) {
	close(client_socket);
	t_recv_messages.detach();
	exit(signal);
}

void client_init(){
	if((client_socket = socket(AF_INET,SOCK_STREAM,0))==-1){ // create peer_socket
		perror("socket: ");
		exit(-1);
	}
	
	server_socket_addr.sin_family = AF_INET; // IPV4
	server_socket_addr.sin_port = htons(10000); // Port no. of server
	server_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Provide IP address of server 
	bzero(&server_socket_addr.sin_zero,(0));

	if((connect(client_socket,(struct sockaddr *)&server_socket_addr,sizeof(struct sockaddr_in)))==-1){
		perror("connect: ");
		close(client_socket);
		exit(-1);
	}
	cout<<colors[NUM_COLORS-2]<<"\n\t  ====== Connected to Server! ======   "<<endl<<reset_col;

	// char client_name[MAX_LEN];
	// cout<<"Name? ";
	// cin.getline(client_name, MAX_LEN);
	// send(client_socket,client_name,sizeof(client_name),0);

	cout<<"Name? "; string client_name; cin>>client_name;
	send_str(client_name, client_socket);
	signal(SIGINT, catch_ctrl_c);
}

// keep receiving messages from server
void recv_messages(){
	while(1){
		string print_str = recv_str(client_socket);
		if(print_str=="#NULL0"){
			cout<<"Error0 detected in client_name"<<endl;
			return;
		}
		else if(print_str=="#NULL1"){
			cout<<"Error1 detected in client_name"<<endl;
			return;
		}
		cout<<print_str<<endl;
		fflush(stdout);
	}	
}

// // keep receiving messages from server
// void recv_messages(){
// 	while(1){
// 		char recvmsg[MAX_LEN];
// 		int bytes_received = recv(client_socket, recvmsg, sizeof(recvmsg), 0);
// 		if(bytes_received==-1){
// 			// handle recieve error
// 			cout<<"Hanlding receive error"<<endl;
// 			break;
// 		}
// 		if(bytes_received==0){ // connection closed by server
// 			cout<<"Connection closed by server"<<endl;
// 			break;
// 		}

// 		string name; string name_col;
// 		int i=0;
// 		for(i=0;i<MAX_LEN;i++){
// 			if(recvmsg[i]==','){
// 				i++;
// 				break;
// 			}
// 			name+=recvmsg[i];
// 		}
// 		for(i;i<MAX_LEN;i++){
// 			name_col+=recvmsg[i];
// 		}
// 		cout<<name_col<<name<<reset_col<<" entered the chat"<<endl;
// 		fflush(stdout);
// 	}	
// }