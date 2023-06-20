#include "headers.h"
int listening_socket; 
struct sockaddr_in listening_socket_addr;

Node tempnode;
vector<Node> clients;
mutex print_mutex;
mutex clients_mutex;

void catch_ctrl_c(int signal) {
	for(int i=0;i<clients.size();i++){
		close(clients[i].socket);
	}
	clients.clear();
	exit(signal);
}

// For synchronisation of cout statements
void shared_print(string str, bool endLine=true){	
	lock_guard<mutex> guard(print_mutex);
	cout<<str;
	if(endLine){
		cout<<endl;
	}
}

void server_init(){
	if((listening_socket = socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket: ");
		exit(-1);
	}
	
	listening_socket_addr.sin_family = AF_INET;
	listening_socket_addr.sin_port = htons(10000); // port address at which tracker will listen
	listening_socket_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&listening_socket_addr.sin_zero,0);

	if((bind(listening_socket,(struct sockaddr *)&listening_socket_addr,sizeof(struct sockaddr_in)))==-1){
		perror("bind error: ");
		exit(-1);
	}

	if((listen(listening_socket,8))==-1){
		perror("listen error: ");
		exit(-1);
	}
	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome Server! ======   "<<endl<<reset_col;

	signal(SIGINT, catch_ctrl_c);
}

void handle_client(int client_id, int client_socket, struct sockaddr_in client_socket_addr){
	string client_name = recv_str(client_socket);
	string printstr = find_color(client_id) + client_name + reset_col + " has joined";
	shared_print(printstr);

	{
		lock_guard<mutex> guard(clients_mutex);	// ensure that the whole message goes as one message in socket; otherwise messages from other server threads can get mixed up in the socket stream
		for(int i=0;i<clients.size();i++){
			if(clients[i].id == client_id) continue;
			send_str(printstr, clients[i].socket);
		}
	}
	{ 	// update clients list
		string name(client_name);
		Node node(client_id, client_socket, name, client_socket_addr);
		lock_guard<mutex> guard(clients_mutex);
		clients.push_back(node);
	}
	// close(client_socket);
}

// void handle_client(int client_id, int client_socket, struct sockaddr_in client_socket_addr){
// 	char client_name[MAX_LEN];
// 	recv(client_socket, client_name, sizeof(client_name), 0);
// 	string printstr = find_color(client_id) + client_name + reset_col + " had joined";
// 	shared_print(printstr);

// 	// send message about arrival of new client to all other clients
// 	for(int i=0;i<clients.size();i++){
// 		if(clients[i].id == client_id) continue;
// 		//cout<<"sending "<<client_name<<" to "<<clients[i].name<<endl;
// 		//send(clients[i].socket,client_name,sizeof(client_name),0);
// 		string to_send_str = client_name;
// 		to_send_str = to_send_str + "," + find_color(clients[i].id);
// 		const char* to_send = to_send_str.c_str(); 
// 		send(clients[i].socket,to_send,sizeof(client_name),0);
// 	}
// 	// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
// 	{ 	// update clients list
// 		string name(client_name);
// 		Node node(client_id, client_socket, name, client_socket_addr);
// 		lock_guard<mutex> guard(add_client_to_list_mutex);
// 		clients.push_back(node);
// 	}
// 	close(client_socket);
// }