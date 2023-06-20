#include "headers.h"
Node client_node;
int client_socket;
struct sockaddr_in server_socket_addr; 
thread t_recv_messages;
thread t_send_messages;

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

	cout<<"Name? "; 
	string client_name; cin>>client_name; //TODO: getline(cin, client_name)
	send_str(client_name, client_socket);


	int client_id;
	ssize_t bytesRead = recv(client_socket, &client_id, sizeof(client_id), 0);
	if(bytesRead<=0){
		cout<<"Error in getting client_id"<<endl;
		return;
	}
	client_node.id = client_id;
	client_node.socket = client_socket;
	client_node.name = client_name;
	signal(SIGINT, catch_ctrl_c);
}

// keep receiving messages from server
void recv_messages(){
	while(1){
		string print_str = recv_str(client_socket);
		if(print_str=="#NULL"){
			cout<<"Error detected in client_name"<<endl;
			return;
		}
		cout<<print_str<<endl;
		fflush(stdout);
	}	
}

// keep sending messages to server
void send_messages(){
	while(1){
		string msg_to_send; 
		getline(cin, msg_to_send);
		string printstr = find_color(client_node.id) + (client_node.name) + ": " + reset_col + msg_to_send;
		send_str(printstr, client_socket);
	}	
}
