#include "myclient.h"

int main(){
	client_init();
	thread t_temp(recv_messages);
	t_recv_messages = move(t_temp);	
	if(t_recv_messages.joinable()){
		t_recv_messages.join();
	}
	return 0;
}