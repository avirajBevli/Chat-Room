#include "myclient.h"

int main(){
	client_init();
	thread t_temp_recv(recv_messages);
	thread t_temp_send(send_messages);
	t_recv_messages = move(t_temp_recv);
	t_send_messages = move(t_temp_send);
	if(t_recv_messages.joinable()){
		t_recv_messages.join();
	}
	if(t_send_messages.joinable()){
		t_send_messages.join();
	}
	return 0;
}
