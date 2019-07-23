/*
 * TestServer.cpp
 *
 *  Created on: 21 июл. 2019 г.
 *      Author: peka
 */


#include "TestServer.h"


namespace TestTask{
//Init udp socket
int server::init_udp_socket(){
	sock_udp_server = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock_udp_server == -1){
		std::cerr<<"Socket UDP Server creation failed..."<<std::endl;
		return sock_udp_server;
	}
	std::cout<<"Socket UDP Server created\n"<<std::endl;
	bzero(&udp_server, sizeof(udp_server));

	udp_server.sin_family = AF_INET;
	udp_server.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_server.sin_port = htons(port_server);

	if(bind(sock_udp_server, reinterpret_cast<struct sockaddr*> (&udp_server), sizeof(udp_server))!=0){
		std::cerr<<"UDP Socket bind failed..."<<std::endl;
	}else{
		std::cout<<"UDP Socket successfully binded..."<<std::endl;
	}
	return sock_udp_server;
}
//Init tcp socket
int server::init_tcp_socket(){
	sock_tcp_server = socket(AF_INET, SOCK_STREAM, 0);

	if(sock_tcp_server == -1){
		std::cerr<<"Socket TCP Server creation failed..."<<std::endl;
		return sock_tcp_server;
	}
	std::cout<<"Socket TCP Server created\n"<<std::endl;
	bzero(&tcp_server, sizeof(tcp_server));

	tcp_server.sin_family = AF_INET;
	tcp_server.sin_addr.s_addr = htonl(INADDR_ANY);
	tcp_server.sin_port = htons(port_server);

	if(bind(sock_tcp_server, reinterpret_cast<struct sockaddr*> (&tcp_server), sizeof(tcp_server))!=0){
		std::cerr<<"TCP Socket bind failed..."<<std::endl;
	}else{
		std::cout<<"TCP Socket successfully binded..."<<std::endl;
	}
	return sock_tcp_server;
}
//TCP Handler for reception message
void server::handler_tcp_recv(int fd){
	char buf_recv[size_buf_recv] = {0};
	std::string result_string="";
	int size_recv = 0;
    fd_set readset;

    while(1){

    	FD_ZERO(&readset);
        FD_SET(fd, &readset);
        //Questioning socket 1 per second
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

		if(select(10, &readset, NULL, NULL, &timeout) <= 0);
		//Check flag on event
		if(FD_ISSET(fd, &readset)){
			size_recv = recv(fd, buf_recv, size_buf_recv, 0);
			if(size_recv <= 0){
				--counter_connection_queue;
				std::cout<<"Connection "<< fd <<  " close" <<std::endl;
				close(fd);
				return;
			}
			for(auto c: buf_recv)
				result_string+=c;
			ParsingMes(result_string);
			std::cout<<result_string<<std::endl;
			send(fd, result_string.c_str(), result_string.size(), 0);
			result_string="";
			memset(buf_recv, 0x00, size_recv+1);
		}
    }
}
//UDP Handler for reception message
void server::handler_udp_recv(int fd){
	char buf_recv[size_buf_recv] = {0};
	std::string result_string="";
	int size_recv = 0;
    fd_set readset;
    struct sockaddr_in remote;
    socklen_t len = sizeof(remote);

    while(1){

    	FD_ZERO(&readset);
        FD_SET(fd, &readset);
        //Questioning socket 1 per second
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

		if(select(10, &readset, NULL, NULL, &timeout) <= 0);
		//Check flag on event
		if(FD_ISSET(fd, &readset)){
			size_recv = recvfrom(fd, buf_recv, size_buf_recv, 0, reinterpret_cast<struct sockaddr*> (&remote), &len);
			if(size_recv <= 0){
				--counter_connection_queue;
				open_socket.erase(fd);
				std::cout<<"Connection "<< fd <<  " close" <<std::endl;
				close(fd);
				return;
			}
			for(auto c: buf_recv)
				result_string+=c;
			ParsingMes(result_string);
			std::cout<<result_string<<std::endl;
			sendto(fd, result_string.c_str(), result_string.size(), 0, reinterpret_cast<struct sockaddr*> (&remote), len);
			result_string="";
			memset(buf_recv, 0x00, size_recv+1);
		}
    }
}
//Handler for processing enter connector - listen TCP socket.
void server::listen_socket(int fd){
    fd_set readset;
    int new_socket;
	listen(fd, size_connection_queue);
	while(1){
        FD_ZERO(&readset);
        FD_SET(fd, &readset);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        if(select(10, &readset, NULL, NULL, &timeout) <= 0);

        if(FD_ISSET(fd, &readset))
        {
        	if(counter_connection_queue<=size_connection_queue){
        		counter_connection_queue++;
        		new_socket = accept(fd, NULL, NULL);
        		open_socket.insert(new_socket);
        		std::cout<<"New client connecting with socket = " << new_socket <<std::endl;
            	queue_connection.emplace_back(new std::thread(&server::handler_tcp_recv, this, new_socket));
        	}else{
        		std::cout<<"Connection limit exceeded!!!" << new_socket <<std::endl;
        	}
        }
	}
}
//Sort numbers and send back message
void server::SendMes(std::vector<int> &BufNum){
	std::string resultString = "";
	int sum = 0;

    std::sort(BufNum.begin(), BufNum.end(), [](int a, int b) {
        return a < b;
    });

	for(auto num : BufNum){
		resultString = resultString + std::to_string(num) + " ";
		sum+=num;
	}
	resultString+=std::to_string(sum);
	std::cout<<resultString<<std::endl;
}

//Extract numbers for message
void server::ParsingMes(std::string& MesStr){
    std::stringstream ss;
    std::vector<int> BufNum;
	std::string resultString = "";
	int sum = 0;

    BufNum.clear();
    ss << MesStr;

    std::string temp;
    int found;
    while (!ss.eof()) {
        ss >> temp;
        if (std::stringstream(temp) >> found)
        	BufNum.push_back(std::stoi(temp));
        temp = "";
    }

    std::sort(BufNum.begin(), BufNum.end(), [](int a, int b) {
        return a < b;
    });

	for(auto num : BufNum){
		resultString = resultString + std::to_string(num) + " ";
		sum+=num;
	}

	resultString+=std::to_string(sum);
	MesStr=resultString;
}

server::server(int port_value)
{
	counter_connection_queue = 0;
	if(port_value < 65535){
		port_server = port_value;
	}else{
		std::cerr<<"Port is not correct...";
		return;
	}
	init_tcp_socket();
	init_udp_socket();
	if(sock_tcp_server!=-1)
		listen_tcp_socket_thread = new std::thread(&server::listen_socket, this, sock_tcp_server);
	if(sock_udp_server!=-1)
		listen_udp_socket_thread = new std::thread(&server::handler_udp_recv, this, sock_udp_server);
}
//Close sockets
server::~server(){
	close(sock_tcp_server);
	close(sock_udp_server);
	for(auto s: open_socket)
		close(s);
	for(auto s: queue_connection)
		s->detach();
}


}
