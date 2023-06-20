#include "headers.h"
Node client_node;
int client_socket;
struct sockaddr_in server_socket_addr; 
thread t_recv_messages;
thread t_send_messages;

bool flag_exit=0;

// Erase text from terminal
void eraseText(int cnt){
	char back_space=8;
	for(int i=0; i<cnt; i++){
		cout<<back_space;
	}	
}

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
	
	cout<<"What is your name: "; 
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

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chatroom "<<client_name<<"! ======   "<<endl<<reset_col;
}

// keep receiving messages from server
void recv_messages(){
	while(1){
		cout<<find_color(client_node.id)<<"You : "<<reset_col; fflush(stdout);
		string print_str = recv_str(client_socket);
		//cout<<print_str<<" received"<<endl;
		if(print_str=="#NULL"){
			cout<<"Error detected in client_name"<<endl;
			t_recv_messages.detach();	
			t_send_messages.detach();
			close(client_socket);
			return;
		}
		eraseText(6);
		cout<<print_str<<endl; fflush(stdout);
	}	
}

// keep sending messages to server
void send_messages(){
	while(1){
		// char str[MAX_LEN];
		// cin.getline(str,MAX_LEN);
		// send(client_socket,str,sizeof(str),0);
		string msg_to_send; cin>>msg_to_send; //TODO: use getline()
		string printstr = find_color(client_node.id) + (client_node.name) + ": " + reset_col + msg_to_send;

		// cout<<"msg_to_send: "<<msg_to_send<<endl;
		if(msg_to_send == "#EXIT"){
			t_recv_messages.detach();	
			t_send_messages.detach();
			close(client_socket);
			return;
		}	
		send_str(printstr, client_socket);
		cout<<find_color(client_node.id)<<"You : "<<reset_col; fflush(stdout);
		// eraseText(6);
	}	
}
