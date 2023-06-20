#include <bits/stdc++.h>
//#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

struct Node{
	int id;
	int socket;
	string name;
	struct sockaddr_in socket_addr; 

	Node(){
		id=-1; socket=-1; name="#NULL"; 
	}

	Node(int a1, int a2, string a3, struct sockaddr_in a4){
		id=a1; socket=a2; name=a3; socket_addr=a4;
	}
};

#define HASH_LEN 5
#define MAX_LEN 200
#define NUM_COLORS 6
string reset_col = "\033[0m";
string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
string find_color(int code){
	return colors[code%NUM_COLORS];
}

string recv_str(int socket_id){
	// Receive an integer value
    int msglen;
    ssize_t bytesRead = recv(socket_id, &msglen, sizeof(msglen), 0);
    if (bytesRead == -1) {
        cout << "Failed to receive the integer value." << endl;
        return "#NULL";
    }
    else if(bytesRead == 0){
    	cout << "read 0 bytes"<<endl;
    	return "#NULL";
    }

    string received_string;
    int msglenrecvd = 0;
    while(msglenrecvd < msglen){
    	char msg_temp[MAX_LEN];
		ssize_t bytesRead = recv(socket_id, msg_temp, sizeof(msg_temp),0);
		if(bytesRead == -1){
			cout << "in while, Failed to receive the integer value." << endl;
        	return "#NULL";
		}
		else if(bytesRead == 0){
	    	cout << "in while, read 0 bytes"<<endl;
	    	return "#NULL";
	    }
		string str_temp(msg_temp);
		received_string += str_temp;
		msglenrecvd+=(str_temp.size());
    }
    // cout<<"msg received: "<<received_string<<"."<<" from "<<socket_id<<endl;
    fflush(stdout);
    return received_string;
}

void send_str(string str, int socket_id){
	int len = str.size();
	send(socket_id, &len, sizeof(len), 0);


	const char* messageData = str.c_str();
	size_t messageLength = str.length();
	size_t totalSent = 0;
    while(totalSent < messageLength) {
        ssize_t bytesSent = send(socket_id, messageData + totalSent, messageLength - totalSent, 0);
        if(bytesSent == -1) {
            cout << "in while, failed to send data." << endl;
            return;
        }
        totalSent += bytesSent;
        // cout<<"totalSent:"<<totalSent<<"  ";
    }
	fflush(stdout);
}
