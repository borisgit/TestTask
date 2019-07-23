/*
 * TestClient.cpp
 *
 *  Created on: 22 июл. 2019 г.
 *      Author: peka
 */


#include "TestClient.hpp"

namespace TestTask{

	client::client(TypeSocket type){
		if(type == TypeSocket::TypeUDP){
			sock_client=socket(AF_INET , SOCK_DGRAM , 0);
			sock_client_port=socket(AF_INET , SOCK_DGRAM , 0);
		}else if(type == TypeSocket::TypeTCP)
			sock_client=socket(AF_INET , SOCK_STREAM , 0);

		if(sock_client==-1)
			std::cerr<<"Socket Client creation failed..."<<std::endl;
		else{
			std::cout<<"Socket Client created\n"<<std::endl;
		}
		port_client = 0;
		typeClient = type;
	}

	bool client::conn(std::string address , int port)
	{
		int sock_recv = 0;
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		inet_pton(AF_INET, address.c_str(), &(server.sin_addr));
		if(typeClient == TypeSocket::TypeTCP){
			if(connect(sock_client, (struct sockaddr *)&server, sizeof(server)) < 0){
				std::cerr<<"Error connect to server!"<<std::endl;
			}
			sock_recv=sock_client;
		}else if(typeClient == TypeSocket::TypeUDP){
			client_port.sin_family = AF_INET;
			client_port.sin_port = htons(port);
			client_port.sin_addr.s_addr = htonl(INADDR_ANY);
			if(bind(sock_client_port,(struct sockaddr*)&client_port,sizeof(client_port))<0){
				std::cerr<<"UDP Socket bind failed..."<<std::endl;
			}else{
				std::cout<<"UDP Socket successfully binded..."<<std::endl;
			}
			sock_recv=sock_client_port;
		}

		listen_socket_thread = new std::thread(&client::handler_receive, this, sock_recv);
	}

	bool client::send_data(std::string data)
	{
		//Send some data
		if(typeClient == TypeSocket::TypeTCP){
			if( send(sock_client , data.c_str() , data.size()  , 0) < 0)
			{
				std::cerr<<"Send failed : "<<std::endl;
				return false;
			}
		}else if(typeClient == TypeSocket::TypeUDP){
		    sendto(sock_client, data.c_str() , data.size(), 0,(struct sockaddr *)&server, sizeof(server));
		}
		std::cout<<"Data send\n";
		return true;
	}

	client::~client(){
		close(sock_client);
	}
	/**
	Receive data from the connected host
	*/
	void client::handler_receive(int fd)
	{
		char buf_recv[size_buf_recv] = {0};
		std::string result_string="";
		int size_recv = 0;
	    fd_set readset;

	    while(1){

	    	FD_ZERO(&readset);
	        FD_SET(fd, &readset);

	        timeval timeout;
	        timeout.tv_sec = 1;
	        timeout.tv_usec = 0;

			if(select(10, &readset, NULL, NULL, &timeout) <= 0);

			if(FD_ISSET(fd, &readset)){
				if(typeClient == TypeSocket::TypeTCP)
					size_recv = recv(fd, buf_recv, size_buf_recv, 0);
				else if(typeClient == TypeSocket::TypeUDP){
					socklen_t buf_len = sizeof(server);
					size_recv = recvfrom(fd, buf_recv, size_buf_recv, 0,/* reinterpret_cast<struct sockaddr*> (&server), &buf_len*/ NULL, NULL);
				}
				if(size_recv>0){
					buf_recv[size_recv] = '\0';
					std::cout<<buf_recv<<std::endl;
				}
			}
	    }
	}
}

