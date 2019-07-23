/*
 * TestServer.h
 *
 *  Created on: 21 июл. 2019 г.
 *      Author: peka
 */

#ifndef TESTSERVER_H_
#define TESTSERVER_H_


#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h> //printf
#include <string> //string
#include <strings.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <set>

namespace TestTask{

	class server{
	private:
		uint sock_tcp_server;
		uint sock_udp_server;
		int port_server;
		struct sockaddr_in tcp_server;
		struct sockaddr_in udp_server;
		int counter_connection_queue;
		static const int size_connection_queue = 10;
		static const int size_buf_recv = 1024;
		void listen_socket(int fd);
		void handler_tcp_recv(int fd);
		void handler_udp_recv(int fd);
		int init_udp_socket();
		int init_tcp_socket();
		void ParsingMes(std::string&);
		void SendMes(std::vector<int>&);
		std::thread *listen_udp_socket_thread;
		std::thread *listen_tcp_socket_thread;
		std::vector <std::shared_ptr<std::thread>> queue_connection;
		std::set<int> open_socket;
	public:
		server(int port_value);
		bool send_data(std::string data);
		std::string receive(int);
		~server();
	};

}



#endif /* TESTSERVER_H_ */
